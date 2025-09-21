#include <am.h>
#include <nemu.h>
#include <klib.h>

uint64_t boot_time;

void __am_timer_init() {
  uint32_t boot_time_msb = inl(0xa0000048 + 4);
  uint32_t boot_time_lsb = inl(0xa0000048);
  boot_time = ((uint64_t)boot_time_msb << 32) | boot_time_lsb;

}

void __am_timer_uptime(AM_TIMER_UPTIME_T *uptime) {
  uint32_t time_msb = inl(0xa0000048 + 4);
  uint32_t time_lsb = inl(0xa0000048);
  uint64_t time = ((uint64_t)time_msb << 32) | time_lsb;
  uptime->us = time - boot_time;
}

void __am_timer_rtc(AM_TIMER_RTC_T *rtc) {
  rtc->second = 0;
  rtc->minute = 0;
  rtc->hour   = 0;
  rtc->day    = 0;
  rtc->month  = 0;
  rtc->year   = 1900;
}
