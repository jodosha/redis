#include "redis.h"
#include <mruby.h>
#include <mruby/proc.h>
#include <mruby/data.h>
#include <mruby/compile.h>

void mrScriptingInit(void) {
  mrb_state *mrb = mrb_open();
  server.mrb = mrb;
}
