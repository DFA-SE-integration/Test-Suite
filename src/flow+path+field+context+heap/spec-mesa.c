/*
 * Heap + field + context + path + flow
 * (mesa-style API tables on heap, current-context switching, branch on function pointer)
 * Author: Sen Ye
 * Date: 09/09/2013
 */
#include "aliascheck.h"
#include <stdlib.h>

__attribute__((noinline))
void begin(int *p, int *q) {
	/* всегда вызываем с &x и &y */
	NOALIAS(p, q);
}

__attribute__((noinline))
void end(int *p, int *q) {
	/* в обеих ветках draw аргументы различны: (&x,&z) или (&y,&x) */
	NOALIAS(p, q);
}

__attribute__((noinline))
void render(int *p, int *q) {
	/* render вызывается только когда CC->API.Render != NULL,
	   и тогда draw подаёт (r, r) */
	MUSTALIAS(p, q);
}

struct api_table {
	void (*Begin)(int *, int *);
	void (*End)(int *, int *);
	void (*Render)(int *, int *);
};

struct context {
	struct api_table API;
	struct api_table Exec;
};

struct mesa_context {
	struct context *ctx;
};

__attribute__((noinline))
void init_exec_pointers(struct api_table *table, int with_render) {
	table->Begin  = begin;
	table->End    = end;
	table->Render = with_render ? render : (void (*)(int*,int*))0;
}

__attribute__((noinline))
void init_api_function(struct context *ctx, int with_render) {
	init_exec_pointers(&ctx->Exec, with_render);
}

__attribute__((noinline))
struct context *create_context(int with_render) {
	struct context *ctx = (struct context*)malloc(sizeof(struct context));
	init_api_function(ctx, with_render);

	/* field-sensitive: API и Exec — разные поля, здесь копирование таблицы */
	ctx->API = ctx->Exec;
	return ctx;
}

struct context *CC;

__attribute__((noinline))
void change_context(struct context *ctx) {
	CC = ctx;
}

__attribute__((noinline))
void make_current(struct mesa_context *mctx) {
	change_context(mctx->ctx);
}

__attribute__((noinline))
void draw(int *p, int *q, int *r) {
	(*CC->API.Begin)(p, q);

	/* эффекты (flow) в отдельных ветках */
	int *q2;
	if (CC->API.Render)
		q2 = r;
	else
		q2 = q;

	/* проверки (path) в отдельных базовых блоках */
	if (CC->API.Render)
		MUSTALIAS(q2, r);
	else
		NOALIAS(q2, r);

	/* ветвление по heap-полю function pointer (path + heap + context) */
	if (CC->API.Render) {
		(*CC->API.Render)(q2, r);  /* (r,r) -> MUSTALIAS внутри render */
		(*CC->API.End)(p, r);      /* (&x,&z) -> NOALIAS внутри end */
	} else {
		(*CC->API.End)(q2, p);     /* (&y,&x) -> NOALIAS внутри end */
	}
}

__attribute__((noinline))
void delete_context(struct context *ctx) {
	free(ctx);
}

int main() {
	int x, y, z;

	/* два heap-контекста с разными API.Render */
	struct mesa_context *m1 = (struct mesa_context*)malloc(sizeof(struct mesa_context));
	struct mesa_context *m2 = (struct mesa_context*)malloc(sizeof(struct mesa_context));
	m1->ctx = create_context(1); /* Render != NULL */
	m2->ctx = create_context(0); /* Render == NULL */

	/* flow-sensitive смена текущего контекста */
	make_current(m1);
	draw(&x, &y, &z);

	make_current(m2);
	draw(&x, &y, &z);

	delete_context(m1->ctx);
	delete_context(m2->ctx);
	free(m1);
	free(m2);

	return 0;
}