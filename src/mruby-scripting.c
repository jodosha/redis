#include "redis.h"
#include <mruby.h>
#include <mruby/proc.h>
#include <mruby/string.h>

void mrAddReply(redisClient *c, mrb_state *mrb, mrb_value value);

void mrScriptingInit(void) {
  mrb_state *mrb = mrb_open();
  server.mrb = mrb;
}

void mrEvalCommand(redisClient *c) {
  mrb_state *mrb = server.mrb;

  struct mrb_parser_state *ps;
  struct RProc *proc;
  mrbc_context *cxt;
  mrb_value val;

  cxt = mrbc_context_new(mrb);
  ps = mrb_parse_string(mrb, c->argv[1]->ptr, cxt);
  proc = mrb_generate_code(mrb, ps);
  mrb_pool_close(ps->pool);

  val = mrb_run(mrb, proc, mrb_top_self(mrb));
  mrAddReply(c, mrb, val);

  mrbc_context_free(mrb, cxt);
}

void mrAddReply(redisClient *c, mrb_state *mrb, mrb_value value) {
  struct RString *str;
  char *s;
  int len;

  value = mrb_inspect(mrb, value);

  if (mrb_string_p(value)) {
    str = mrb_str_ptr(value);
    s = str->ptr;
    len = str->len;
    addReplyBulkCBuffer(c, s, len);
  }

}
