#include <am.h>
#include <klib.h>

uint64_t boot_time;

void __am_timer_init() {
  uint32_t boot_time_msb = inl(RTC_ADDR + 4);
  uint32_t boot_time_lsb = inl(RTC_ADDR);
  boot_time = ((uint64_t)boot_time_msb << 32) | boot_time_lsb;
}

void __am_timer_uptime(AM_TIMER_UPTIME_T *uptime) {
  uptime->us = 0;
}

void __am_timer_rtc(AM_TIMER_RTC_T *rtc) {
  rtc->second = 0;
  rtc->minute = 0;
  rtc->hour   = 0;
  rtc->day    = 0;
  rtc->month  = 0;
  rtc->year   = 1900;
}
