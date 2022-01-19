#pragma once
#ifndef __PIC32M_H__
#define __PIC32M_H__

#ifndef __LANGUAGE_ASSEMBLY__
#include <stdint.h>
#endif

/* PIC32MX5xx */
#  if defined(__32MX530F128H__) || defined(__PIC32MX530F128H__)
# include <proc/PIC32MX/p32mx530f128h.h>
#elif defined(__32MX530F128L__) || defined(__PIC32MX530F128L__)
# include <proc/PIC32MX/p32mx530f128l.h>
#elif defined(__32MX534F064H__) || defined(__PIC32MX534F064H__)
# include <proc/PIC32MX/p32mx534f064h.h>
#elif defined(__32MX534F064L__) || defined(__PIC32MX534F064L__)
# include <proc/PIC32MX/p32mx534f064l.h>
#elif defined(__32MX550F256H__) || defined(__PIC32MX550F256H__)
# include <proc/PIC32MX/p32mx550f256h.h>
#elif defined(__32MX550F256L__) || defined(__PIC32MX550F256L__)
# include <proc/PIC32MX/p32mx550f256l.h>
#elif defined(__32MX564F064H__) || defined(__PIC32MX564F064H__)
# include <proc/PIC32MX/p32mx564f064h.h>
#elif defined(__32MX564F064L__) || defined(__PIC32MX564F064L__)
# include <proc/PIC32MX/p32mx564f064l.h>
#elif defined(__32MX564F128H__) || defined(__PIC32MX564F128H__)
# include <proc/PIC32MX/p32mx564f128h.h>
#elif defined(__32MX564F128L__) || defined(__PIC32MX564F128L__)
# include <proc/PIC32MX/p32mx564f128l.h>
#elif defined(__32MX570F512H__) || defined(__PIC32MX570F512H__)
# include <proc/PIC32MX/p32mx570f512h.h>
#elif defined(__32MX570F512L__) || defined(__PIC32MX570F512L__)
# include <proc/PIC32MX/p32mx570f512l.h>
#elif defined(__32MX575F256H__) || defined(__PIC32MX575F256H__)
# include <proc/PIC32MX/p32mx575f256h.h>
#elif defined(__32MX575F256L__) || defined(__PIC32MX575F256L__)
# include <proc/PIC32MX/p32mx575f256l.h>
#elif defined(__32MX575F512H__) || defined(__PIC32MX575F512H__)
# include <proc/PIC32MX/p32mx575f512h.h>
#elif defined(__32MX575F512L__) || defined(__PIC32MX575F512L__)
# include <proc/PIC32MX/p32mx575f512l.h>
#elif defined(__32MX664F064H__) || defined(__PIC32MX664F064H__)
# include <proc/PIC32MX/p32mx664f064h.h>
#elif defined(__32MX664F064L__) || defined(__PIC32MX664F064L__)
# include <proc/PIC32MX/p32mx664f064l.h>
#elif defined(__32MX664F128H__) || defined(__PIC32MX664F128H__)
# include <proc/PIC32MX/p32mx664f128h.h>
#elif defined(__32MX664F128L__) || defined(__PIC32MX664F128L__)
# include <proc/PIC32MX/p32mx664f128l.h>
#elif defined(__32MX675F256H__) || defined(__PIC32MX675F256H__)
# include <proc/PIC32MX/p32mx675f256h.h>
#elif defined(__32MX675F256L__) || defined(__PIC32MX675F256L__)
# include <proc/PIC32MX/p32mx675f256l.h>
#elif defined(__32MX675F512H__) || defined(__PIC32MX675F512H__)
# include <proc/PIC32MX/p32mx675f512h.h>
#elif defined(__32MX675F512L__) || defined(__PIC32MX675F512L__)
# include <proc/PIC32MX/p32mx675f512l.h>
#elif defined(__32MX695F512H__) || defined(__PIC32MX695F512H__)
# include <proc/PIC32MX/p32mx695f512h.h>
#elif defined(__32MX695F512L__) || defined(__PIC32MX695F512L__)
# include <proc/PIC32MX/p32mx695f512l.h>
#elif defined(__32MX764F128H__) || defined(__PIC32MX764F128H__)
# include <proc/PIC32MX/p32mx764f128h.h>
#elif defined(__32MX764F128L__) || defined(__PIC32MX764F128L__)
# include <proc/PIC32MX/p32mx764f128l.h>
#elif defined(__32MX775F256H__) || defined(__PIC32MX775F256H__)
# include <proc/PIC32MX/p32mx775f256h.h>
#elif defined(__32MX775F256L__) || defined(__PIC32MX775F256L__)
# include <proc/PIC32MX/p32mx775f256l.h>
#elif defined(__32MX775F512H__) || defined(__PIC32MX775F512H__)
# include <proc/PIC32MX/p32mx775f512h.h>
#elif defined(__32MX775F512L__) || defined(__PIC32MX775F512L__)
# include <proc/PIC32MX/p32mx775f512l.h>
#elif defined(__32MX795F512H__) || defined(__PIC32MX795F512H__)
# include <proc/PIC32MX/p32mx795f512h.h>
#elif defined(__32MX795F512L__) || defined(__PIC32MX795F512L__)
# include <proc/PIC32MX/p32mx795f512l.h>
/* PIC32MZ-DA */
#elif defined(__32MZ1025DAA169__) || defined(__PIC32MZ1025DAA169__)
# include <proc/PIC32MZ-DA/p32mz1025daa169.h>
#elif defined(__32MZ1025DAA176__) || defined(__PIC32MZ1025DAA176__)
# include <proc/PIC32MZ-DA/p32mz1025daa176.h>
#elif defined(__32MZ1025DAA288__) || defined(__PIC32MZ1025DAA288__)
# include <proc/PIC32MZ-DA/p32mz1025daa288.h>
#elif defined(__32MZ1025DAB169__) || defined(__PIC32MZ1025DAB169__)
# include <proc/PIC32MZ-DA/p32mz1025dab169.h>
#elif defined(__32MZ1025DAB176__) || defined(__PIC32MZ1025DAB176__)
# include <proc/PIC32MZ-DA/p32mz1025dab176.h>
#elif defined(__32MZ1025DAB288__) || defined(__PIC32MZ1025DAB288__)
# include <proc/PIC32MZ-DA/p32mz1025dab288.h>
#elif defined(__32MZ1025DAG169__) || defined(__PIC32MZ1025DAG169__)
# include <proc/PIC32MZ-DA/p32mz1025dag169.h>
#elif defined(__32MZ1025DAG176__) || defined(__PIC32MZ1025DAG176__)
# include <proc/PIC32MZ-DA/p32mz1025dag176.h>
#elif defined(__32MZ1025DAH169__) || defined(__PIC32MZ1025DAH169__)
# include <proc/PIC32MZ-DA/p32mz1025dah169.h>
#elif defined(__32MZ1025DAH176__) || defined(__PIC32MZ1025DAH176__)
# include <proc/PIC32MZ-DA/p32mz1025dah176.h>
#elif defined(__32MZ1025DAK169__) || defined(__PIC32MZ1025DAK169__)
# include <proc/PIC32MZ-DA/p32mz1025dak169.h>
#elif defined(__32MZ1025DAK176__) || defined(__PIC32MZ1025DAK176__)
# include <proc/PIC32MZ-DA/p32mz1025dak176.h>
#elif defined(__32MZ1025DAL169__) || defined(__PIC32MZ1025DAL169__)
# include <proc/PIC32MZ-DA/p32mz1025dal169.h>
#elif defined(__32MZ1025DAL176__) || defined(__PIC32MZ1025DAL176__)
# include <proc/PIC32MZ-DA/p32mz1025dal176.h>
#elif defined(__32MZ1025DAR169__) || defined(__PIC32MZ1025DAR169__)
# include <proc/PIC32MZ-DA/p32mz1025dar169.h>
#elif defined(__32MZ1025DAR176__) || defined(__PIC32MZ1025DAR176__)
# include <proc/PIC32MZ-DA/p32mz1025dar176.h>
#elif defined(__32MZ1025DAS169__) || defined(__PIC32MZ1025DAS169__)
# include <proc/PIC32MZ-DA/p32mz1025das169.h>
#elif defined(__32MZ1025DAS176__) || defined(__PIC32MZ1025DAS176__)
# include <proc/PIC32MZ-DA/p32mz1025das176.h>
#elif defined(__32MZ1064DAA169__) || defined(__PIC32MZ1064DAA169__)
# include <proc/PIC32MZ-DA/p32mz1064daa169.h>
#elif defined(__32MZ1064DAA176__) || defined(__PIC32MZ1064DAA176__)
# include <proc/PIC32MZ-DA/p32mz1064daa176.h>
#elif defined(__32MZ1064DAA288__) || defined(__PIC32MZ1064DAA288__)
# include <proc/PIC32MZ-DA/p32mz1064daa288.h>
#elif defined(__32MZ1064DAB169__) || defined(__PIC32MZ1064DAB169__)
# include <proc/PIC32MZ-DA/p32mz1064dab169.h>
#elif defined(__32MZ1064DAB176__) || defined(__PIC32MZ1064DAB176__)
# include <proc/PIC32MZ-DA/p32mz1064dab176.h>
#elif defined(__32MZ1064DAB288__) || defined(__PIC32MZ1064DAB288__)
# include <proc/PIC32MZ-DA/p32mz1064dab288.h>
#elif defined(__32MZ1064DAG169__) || defined(__PIC32MZ1064DAG169__)
# include <proc/PIC32MZ-DA/p32mz1064dag169.h>
#elif defined(__32MZ1064DAG176__) || defined(__PIC32MZ1064DAG176__)
# include <proc/PIC32MZ-DA/p32mz1064dag176.h>
#elif defined(__32MZ1064DAH169__) || defined(__PIC32MZ1064DAH169__)
# include <proc/PIC32MZ-DA/p32mz1064dah169.h>
#elif defined(__32MZ1064DAH176__) || defined(__PIC32MZ1064DAH176__)
# include <proc/PIC32MZ-DA/p32mz1064dah176.h>
#elif defined(__32MZ1064DAK169__) || defined(__PIC32MZ1064DAK169__)
# include <proc/PIC32MZ-DA/p32mz1064dak169.h>
#elif defined(__32MZ1064DAK176__) || defined(__PIC32MZ1064DAK176__)
# include <proc/PIC32MZ-DA/p32mz1064dak176.h>
#elif defined(__32MZ1064DAL169__) || defined(__PIC32MZ1064DAL169__)
# include <proc/PIC32MZ-DA/p32mz1064dal169.h>
#elif defined(__32MZ1064DAL176__) || defined(__PIC32MZ1064DAL176__)
# include <proc/PIC32MZ-DA/p32mz1064dal176.h>
#elif defined(__32MZ1064DAR169__) || defined(__PIC32MZ1064DAR169__)
# include <proc/PIC32MZ-DA/p32mz1064dar169.h>
#elif defined(__32MZ1064DAR176__) || defined(__PIC32MZ1064DAR176__)
# include <proc/PIC32MZ-DA/p32mz1064dar176.h>
#elif defined(__32MZ1064DAS169__) || defined(__PIC32MZ1064DAS169__)
# include <proc/PIC32MZ-DA/p32mz1064das169.h>
#elif defined(__32MZ1064DAS176__) || defined(__PIC32MZ1064DAS176__)
# include <proc/PIC32MZ-DA/p32mz1064das176.h>
#elif defined(__32MZ2025DAA169__) || defined(__PIC32MZ2025DAA169__)
# include <proc/PIC32MZ-DA/p32mz2025daa169.h>
#elif defined(__32MZ2025DAA176__) || defined(__PIC32MZ2025DAA176__)
# include <proc/PIC32MZ-DA/p32mz2025daa176.h>
#elif defined(__32MZ2025DAA288__) || defined(__PIC32MZ2025DAA288__)
# include <proc/PIC32MZ-DA/p32mz2025daa288.h>
#elif defined(__32MZ2025DAB169__) || defined(__PIC32MZ2025DAB169__)
# include <proc/PIC32MZ-DA/p32mz2025dab169.h>
#elif defined(__32MZ2025DAB176__) || defined(__PIC32MZ2025DAB176__)
# include <proc/PIC32MZ-DA/p32mz2025dab176.h>
#elif defined(__32MZ2025DAB288__) || defined(__PIC32MZ2025DAB288__)
# include <proc/PIC32MZ-DA/p32mz2025dab288.h>
#elif defined(__32MZ2025DAG169__) || defined(__PIC32MZ2025DAG169__)
# include <proc/PIC32MZ-DA/p32mz2025dag169.h>
#elif defined(__32MZ2025DAG176__) || defined(__PIC32MZ2025DAG176__)
# include <proc/PIC32MZ-DA/p32mz2025dag176.h>
#elif defined(__32MZ2025DAH169__) || defined(__PIC32MZ2025DAH169__)
# include <proc/PIC32MZ-DA/p32mz2025dah169.h>
#elif defined(__32MZ2025DAH176__) || defined(__PIC32MZ2025DAH176__)
# include <proc/PIC32MZ-DA/p32mz2025dah176.h>
#elif defined(__32MZ2025DAK169__) || defined(__PIC32MZ2025DAK169__)
# include <proc/PIC32MZ-DA/p32mz2025dak169.h>
#elif defined(__32MZ2025DAK176__) || defined(__PIC32MZ2025DAK176__)
# include <proc/PIC32MZ-DA/p32mz2025dak176.h>
#elif defined(__32MZ2025DAL169__) || defined(__PIC32MZ2025DAL169__)
# include <proc/PIC32MZ-DA/p32mz2025dal169.h>
#elif defined(__32MZ2025DAL176__) || defined(__PIC32MZ2025DAL176__)
# include <proc/PIC32MZ-DA/p32mz2025dal176.h>
#elif defined(__32MZ2025DAR169__) || defined(__PIC32MZ2025DAR169__)
# include <proc/PIC32MZ-DA/p32mz2025dar169.h>
#elif defined(__32MZ2025DAR176__) || defined(__PIC32MZ2025DAR176__)
# include <proc/PIC32MZ-DA/p32mz2025dar176.h>
#elif defined(__32MZ2025DAS169__) || defined(__PIC32MZ2025DAS169__)
# include <proc/PIC32MZ-DA/p32mz2025das169.h>
#elif defined(__32MZ2025DAS176__) || defined(__PIC32MZ2025DAS176__)
# include <proc/PIC32MZ-DA/p32mz2025das176.h>
#elif defined(__32MZ2064DAA169__) || defined(__PIC32MZ2064DAA169__)
# include <proc/PIC32MZ-DA/p32mz2064daa169.h>
#elif defined(__32MZ2064DAA176__) || defined(__PIC32MZ2064DAA176__)
# include <proc/PIC32MZ-DA/p32mz2064daa176.h>
#elif defined(__32MZ2064DAA288__) || defined(__PIC32MZ2064DAA288__)
# include <proc/PIC32MZ-DA/p32mz2064daa288.h>
#elif defined(__32MZ2064DAB169__) || defined(__PIC32MZ2064DAB169__)
# include <proc/PIC32MZ-DA/p32mz2064dab169.h>
#elif defined(__32MZ2064DAB176__) || defined(__PIC32MZ2064DAB176__)
# include <proc/PIC32MZ-DA/p32mz2064dab176.h>
#elif defined(__32MZ2064DAB288__) || defined(__PIC32MZ2064DAB288__)
# include <proc/PIC32MZ-DA/p32mz2064dab288.h>
#elif defined(__32MZ2064DAG169__) || defined(__PIC32MZ2064DAG169__)
# include <proc/PIC32MZ-DA/p32mz2064dag169.h>
#elif defined(__32MZ2064DAG176__) || defined(__PIC32MZ2064DAG176__)
# include <proc/PIC32MZ-DA/p32mz2064dag176.h>
#elif defined(__32MZ2064DAH169__) || defined(__PIC32MZ2064DAH169__)
# include <proc/PIC32MZ-DA/p32mz2064dah169.h>
#elif defined(__32MZ2064DAH176__) || defined(__PIC32MZ2064DAH176__)
# include <proc/PIC32MZ-DA/p32mz2064dah176.h>
#elif defined(__32MZ2064DAK169__) || defined(__PIC32MZ2064DAK169__)
# include <proc/PIC32MZ-DA/p32mz2064dak169.h>
#elif defined(__32MZ2064DAK176__) || defined(__PIC32MZ2064DAK176__)
# include <proc/PIC32MZ-DA/p32mz2064dak176.h>
#elif defined(__32MZ2064DAL169__) || defined(__PIC32MZ2064DAL169__)
# include <proc/PIC32MZ-DA/p32mz2064dal169.h>
#elif defined(__32MZ2064DAL176__) || defined(__PIC32MZ2064DAL176__)
# include <proc/PIC32MZ-DA/p32mz2064dal176.h>
#elif defined(__32MZ2064DAR169__) || defined(__PIC32MZ2064DAR169__)
# include <proc/PIC32MZ-DA/p32mz2064dar169.h>
#elif defined(__32MZ2064DAR176__) || defined(__PIC32MZ2064DAR176__)
# include <proc/PIC32MZ-DA/p32mz2064dar176.h>
#elif defined(__32MZ2064DAS169__) || defined(__PIC32MZ2064DAS169__)
# include <proc/PIC32MZ-DA/p32mz2064das169.h>
#elif defined(__32MZ2064DAS176__) || defined(__PIC32MZ2064DAS176__)
# include <proc/PIC32MZ-DA/p32mz2064das176.h>
/* PIC32MZ-EC */
#elif defined(__32MZ1024ECG064__) || defined(__PIC32MZ1024ECG064__)
# include <proc/PIC32MZ-EC/p32mz1024ecg064.h>
#elif defined(__32MZ1024ECG100__) || defined(__PIC32MZ1024ECG100__)
# include <proc/PIC32MZ-EC/p32mz1024ecg100.h>
#elif defined(__32MZ1024ECG124__) || defined(__PIC32MZ1024ECG124__)
# include <proc/PIC32MZ-EC/p32mz1024ecg124.h>
#elif defined(__32MZ1024ECG144__) || defined(__PIC32MZ1024ECG144__)
# include <proc/PIC32MZ-EC/p32mz1024ecg144.h>
#elif defined(__32MZ1024ECH064__) || defined(__PIC32MZ1024ECH064__)
# include <proc/PIC32MZ-EC/p32mz1024ech064.h>
#elif defined(__32MZ1024ECH100__) || defined(__PIC32MZ1024ECH100__)
# include <proc/PIC32MZ-EC/p32mz1024ech100.h>
#elif defined(__32MZ1024ECH124__) || defined(__PIC32MZ1024ECH124__)
# include <proc/PIC32MZ-EC/p32mz1024ech124.h>
#elif defined(__32MZ1024ECH144__) || defined(__PIC32MZ1024ECH144__)
# include <proc/PIC32MZ-EC/p32mz1024ech144.h>
#elif defined(__32MZ1024ECM064__) || defined(__PIC32MZ1024ECM064__)
# include <proc/PIC32MZ-EC/p32mz1024ecm064.h>
#elif defined(__32MZ1024ECM100__) || defined(__PIC32MZ1024ECM100__)
# include <proc/PIC32MZ-EC/p32mz1024ecm100.h>
#elif defined(__32MZ1024ECM124__) || defined(__PIC32MZ1024ECM124__)
# include <proc/PIC32MZ-EC/p32mz1024ecm124.h>
#elif defined(__32MZ1024ECM144__) || defined(__PIC32MZ1024ECM144__)
# include <proc/PIC32MZ-EC/p32mz1024ecm144.h>
#elif defined(__32MZ2048ECG064__) || defined(__PIC32MZ2048ECG064__)
# include <proc/PIC32MZ-EC/p32mz2048ecg064.h>
#elif defined(__32MZ2048ECG100__) || defined(__PIC32MZ2048ECG100__)
# include <proc/PIC32MZ-EC/p32mz2048ecg100.h>
#elif defined(__32MZ2048ECG124__) || defined(__PIC32MZ2048ECG124__)
# include <proc/PIC32MZ-EC/p32mz2048ecg124.h>
#elif defined(__32MZ2048ECG144__) || defined(__PIC32MZ2048ECG144__)
# include <proc/PIC32MZ-EC/p32mz2048ecg144.h>
#elif defined(__32MZ2048ECH064__) || defined(__PIC32MZ2048ECH064__)
# include <proc/PIC32MZ-EC/p32mz2048ech064.h>
#elif defined(__32MZ2048ECH100__) || defined(__PIC32MZ2048ECH100__)
# include <proc/PIC32MZ-EC/p32mz2048ech100.h>
#elif defined(__32MZ2048ECH124__) || defined(__PIC32MZ2048ECH124__)
# include <proc/PIC32MZ-EC/p32mz2048ech124.h>
#elif defined(__32MZ2048ECH144__) || defined(__PIC32MZ2048ECH144__)
# include <proc/PIC32MZ-EC/p32mz2048ech144.h>
#elif defined(__32MZ2048ECM064__) || defined(__PIC32MZ2048ECM064__)
# include <proc/PIC32MZ-EC/p32mz2048ecm064.h>
#elif defined(__32MZ2048ECM100__) || defined(__PIC32MZ2048ECM100__)
# include <proc/PIC32MZ-EC/p32mz2048ecm100.h>
#elif defined(__32MZ2048ECM124__) || defined(__PIC32MZ2048ECM124__)
# include <proc/PIC32MZ-EC/p32mz2048ecm124.h>
#elif defined(__32MZ2048ECM144__) || defined(__PIC32MZ2048ECM144__)
# include <proc/PIC32MZ-EC/p32mz2048ecm144.h>
/* PIC32MZ-EF */
#elif defined(__32MZ0512EFE064__) || defined(__PIC32MZ0512EFE064__)
# include <proc/PIC32MZ-EF/p32mz0512efe064.h>
#elif defined(__32MZ0512EFE100__) || defined(__PIC32MZ0512EFE100__)
# include <proc/PIC32MZ-EF/p32mz0512efe100.h>
#elif defined(__32MZ0512EFE124__) || defined(__PIC32MZ0512EFE124__)
# include <proc/PIC32MZ-EF/p32mz0512efe124.h>
#elif defined(__32MZ0512EFE144__) || defined(__PIC32MZ0512EFE144__)
# include <proc/PIC32MZ-EF/p32mz0512efe144.h>
#elif defined(__32MZ0512EFF064__) || defined(__PIC32MZ0512EFF064__)
# include <proc/PIC32MZ-EF/p32mz0512eff064.h>
#elif defined(__32MZ0512EFF100__) || defined(__PIC32MZ0512EFF100__)
# include <proc/PIC32MZ-EF/p32mz0512eff100.h>
#elif defined(__32MZ0512EFF124__) || defined(__PIC32MZ0512EFF124__)
# include <proc/PIC32MZ-EF/p32mz0512eff124.h>
#elif defined(__32MZ0512EFF144__) || defined(__PIC32MZ0512EFF144__)
# include <proc/PIC32MZ-EF/p32mz0512eff144.h>
#elif defined(__32MZ0512EFK064__) || defined(__PIC32MZ0512EFK064__)
# include <proc/PIC32MZ-EF/p32mz0512efk064.h>
#elif defined(__32MZ0512EFK100__) || defined(__PIC32MZ0512EFK100__)
# include <proc/PIC32MZ-EF/p32mz0512efk100.h>
#elif defined(__32MZ0512EFK124__) || defined(__PIC32MZ0512EFK124__)
# include <proc/PIC32MZ-EF/p32mz0512efk124.h>
#elif defined(__32MZ0512EFK144__) || defined(__PIC32MZ0512EFK144__)
# include <proc/PIC32MZ-EF/p32mz0512efk144.h>
#elif defined(__32MZ1024EFE064__) || defined(__PIC32MZ1024EFE064__)
# include <proc/PIC32MZ-EF/p32mz1024efe064.h>
#elif defined(__32MZ1024EFE100__) || defined(__PIC32MZ1024EFE100__)
# include <proc/PIC32MZ-EF/p32mz1024efe100.h>
#elif defined(__32MZ1024EFE124__) || defined(__PIC32MZ1024EFE124__)
# include <proc/PIC32MZ-EF/p32mz1024efe124.h>
#elif defined(__32MZ1024EFE144__) || defined(__PIC32MZ1024EFE144__)
# include <proc/PIC32MZ-EF/p32mz1024efe144.h>
#elif defined(__32MZ1024EFF064__) || defined(__PIC32MZ1024EFF064__)
# include <proc/PIC32MZ-EF/p32mz1024eff064.h>
#elif defined(__32MZ1024EFF100__) || defined(__PIC32MZ1024EFF100__)
# include <proc/PIC32MZ-EF/p32mz1024eff100.h>
#elif defined(__32MZ1024EFF124__) || defined(__PIC32MZ1024EFF124__)
# include <proc/PIC32MZ-EF/p32mz1024eff124.h>
#elif defined(__32MZ1024EFF144__) || defined(__PIC32MZ1024EFF144__)
# include <proc/PIC32MZ-EF/p32mz1024eff144.h>
#elif defined(__32MZ1024EFG064__) || defined(__PIC32MZ1024EFG064__)
# include <proc/PIC32MZ-EF/p32mz1024efg064.h>
#elif defined(__32MZ1024EFG100__) || defined(__PIC32MZ1024EFG100__)
# include <proc/PIC32MZ-EF/p32mz1024efg100.h>
#elif defined(__32MZ1024EFG124__) || defined(__PIC32MZ1024EFG124__)
# include <proc/PIC32MZ-EF/p32mz1024efg124.h>
#elif defined(__32MZ1024EFG144__) || defined(__PIC32MZ1024EFG144__)
# include <proc/PIC32MZ-EF/p32mz1024efg144.h>
#elif defined(__32MZ1024EFH064__) || defined(__PIC32MZ1024EFH064__)
# include <proc/PIC32MZ-EF/p32mz1024efh064.h>
#elif defined(__32MZ1024EFH100__) || defined(__PIC32MZ1024EFH100__)
# include <proc/PIC32MZ-EF/p32mz1024efh100.h>
#elif defined(__32MZ1024EFH124__) || defined(__PIC32MZ1024EFH124__)
# include <proc/PIC32MZ-EF/p32mz1024efh124.h>
#elif defined(__32MZ1024EFH144__) || defined(__PIC32MZ1024EFH144__)
# include <proc/PIC32MZ-EF/p32mz1024efh144.h>
#elif defined(__32MZ1024EFK064__) || defined(__PIC32MZ1024EFK064__)
# include <proc/PIC32MZ-EF/p32mz1024efk064.h>
#elif defined(__32MZ1024EFK100__) || defined(__PIC32MZ1024EFK100__)
# include <proc/PIC32MZ-EF/p32mz1024efk100.h>
#elif defined(__32MZ1024EFK124__) || defined(__PIC32MZ1024EFK124__)
# include <proc/PIC32MZ-EF/p32mz1024efk124.h>
#elif defined(__32MZ1024EFK144__) || defined(__PIC32MZ1024EFK144__)
# include <proc/PIC32MZ-EF/p32mz1024efk144.h>
#elif defined(__32MZ1024EFM064__) || defined(__PIC32MZ1024EFM064__)
# include <proc/PIC32MZ-EF/p32mz1024efm064.h>
#elif defined(__32MZ1024EFM100__) || defined(__PIC32MZ1024EFM100__)
# include <proc/PIC32MZ-EF/p32mz1024efm100.h>
#elif defined(__32MZ1024EFM124__) || defined(__PIC32MZ1024EFM124__)
# include <proc/PIC32MZ-EF/p32mz1024efm124.h>
#elif defined(__32MZ1024EFM144__) || defined(__PIC32MZ1024EFM144__)
# include <proc/PIC32MZ-EF/p32mz1024efm144.h>
#elif defined(__32MZ2048EFG064__) || defined(__PIC32MZ2048EFG064__)
# include <proc/PIC32MZ-EF/p32mz2048efg064.h>
#elif defined(__32MZ2048EFG100__) || defined(__PIC32MZ2048EFG100__)
# include <proc/PIC32MZ-EF/p32mz2048efg100.h>
#elif defined(__32MZ2048EFG124__) || defined(__PIC32MZ2048EFG124__)
# include <proc/PIC32MZ-EF/p32mz2048efg124.h>
#elif defined(__32MZ2048EFG144__) || defined(__PIC32MZ2048EFG144__)
# include <proc/PIC32MZ-EF/p32mz2048efg144.h>
#elif defined(__32MZ2048EFH064__) || defined(__PIC32MZ2048EFH064__)
# include <proc/PIC32MZ-EF/p32mz2048efh064.h>
#elif defined(__32MZ2048EFH100__) || defined(__PIC32MZ2048EFH100__)
# include <proc/PIC32MZ-EF/p32mz2048efh100.h>
#elif defined(__32MZ2048EFH124__) || defined(__PIC32MZ2048EFH124__)
# include <proc/PIC32MZ-EF/p32mz2048efh124.h>
#elif defined(__32MZ2048EFH144__) || defined(__PIC32MZ2048EFH144__)
# include <proc/PIC32MZ-EF/p32mz2048efh144.h>
#elif defined(__32MZ2048EFM064__) || defined(__PIC32MZ2048EFM064__)
# include <proc/PIC32MZ-EF/p32mz2048efm064.h>
#elif defined(__32MZ2048EFM100__) || defined(__PIC32MZ2048EFM100__)
# include <proc/PIC32MZ-EF/p32mz2048efm100.h>
#elif defined(__32MZ2048EFM124__) || defined(__PIC32MZ2048EFM124__)
# include <proc/PIC32MZ-EF/p32mz2048efm124.h>
#elif defined(__32MZ2048EFM144__) || defined(__PIC32MZ2048EFM144__)
# include <proc/PIC32MZ-EF/p32mz2048efm144.h>
/* PIC32MZ-W */
#elif defined(__32MZ1025W104132__) || defined(__PIC32MZ1025W104132__)
# include <proc/PIC32MZ-W/p32mz1025w104132.h>
#else
#  error Unknown processor!
#endif

#ifdef __LANGUAGE_ASSEMBLY__

#define zero               $0

#define at_reg             $1  // assembler temporary, reserved for
#define AT_reg             $1

#define v0                 $2  // return value
#define v1                 $3

#define a0                 $4  // first args of sub
#define a1                 $5
#define a2                 $6
#define a3                 $7

#define t0                 $8  // local variables, not preserved
#define t1                 $9
#define t2                 $10
#define t3                 $11
#define t4                 $12
#define t5                 $13
#define t6                 $14
#define t7                 $15
#define t8                 $24
#define t9                 $25

#define s0                 $16  // local variables, must be preserved
#define s1                 $17
#define s2                 $18
#define s3                 $19
#define s4                 $20
#define s5                 $21
#define s6                 $22
#define s7                 $23

#define s8                 $30  // frame pointer
#define fp                 $30

#define k0                 $26  // reserved for use by interrupt/trap handler
#define k1                 $27

#define gp                 $28  // global pointer
#define sp                 $29  // stack pointer
#define ra                 $31  // return address

#endif

#if defined (__LANGUAGE_C__) || defined (__LANGUAGE_C_PLUS_PLUS__)

#ifndef _nop
  #ifdef __mips16
    #define _nop()  __asm__ __volatile__ (" move $0,$16" : :)
  #else
    #define _nop()  __asm__ __volatile__ ("%(ssnop%)" : :)
  #endif
#endif

#ifndef _ehb
#define _ehb()  __asm__ __volatile__ ("%(ehb%)" : :)
#endif

#ifdef __mips16
extern void __attribute__((__nomips16__)) _sync(void);
#else
#define _sync() __asm__ __volatile__ ("sync" : : : "memory")
#endif

#define _wait() __asm__ __volatile__ ("wait")


#if defined(__GCC_HAVE_BUILTIN_MIPS_CACHE) && defined(__PIC32_HAS_L1CACHE)
  #define _cache(op,addr)      __builtin_mips_cache(op,addr)
#else
  #define _cache(op,addr)
#endif

#if defined(__PIC32_HAS_L1CACHE)
  #define _synci(x) __extension__({ \
       __asm__ ("synci %a0" :: "d" (x)); \
   })
#else
  #define _synci(addr)
#endif

#if defined(__PIC32_HAS_L1CACHE)
  #define _prefetch(hint,x) __extension__({ \
      __asm__ ("pref %0, %a1" :: "JYA" (hint), "d" (x) : "memory"); \
   })
#else
  #define _prefetch(hint,addr)
#endif


#define __gen_fset( __fun ) \
__fun(  0, 0 ) \
__fun(  1, 0 ) \
__fun(  2, 0 ) \
__fun(  3, 0 ) \
__fun(  4, 0 ) \
__fun(  4, 2 ) \
__fun(  5, 0 ) \
__fun(  5, 1 ) \
__fun(  6, 0 ) \
__fun(  7, 0 ) \
__fun(  8, 0 ) \
__fun(  9, 0 ) \
__fun( 10, 0 ) \
__fun( 11, 0 ) \
__fun( 12, 0 ) \
__fun( 12, 1 ) \
__fun( 12, 2 ) \
__fun( 12, 3 ) \
__fun( 12, 4 ) \
__fun( 12, 5 ) \
__fun( 13, 0 ) \
__fun( 13, 1 ) \
__fun( 13, 2 ) \
__fun( 14, 0 ) \
__fun( 14, 2 ) \
__fun( 15, 0 ) \
__fun( 15, 1 ) \
__fun( 15, 2 ) \
__fun( 16, 0 ) \
__fun( 16, 1 ) \
__fun( 16, 2 ) \
__fun( 16, 3 ) \
__fun( 16, 4 ) \
__fun( 16, 5 ) \
__fun( 16, 7 ) \
__fun( 17, 0 ) \
__fun( 18, 0 ) \
__fun( 19, 0 ) \
__fun( 23, 0 ) \
__fun( 23, 1 ) \
__fun( 23, 2 ) \
__fun( 23, 3 ) \
__fun( 23, 4 ) \
__fun( 23, 5 ) \
__fun( 24, 0 ) \
__fun( 24, 1 ) \
__fun( 25, 0 ) \
__fun( 25, 1 ) \
__fun( 25, 2 ) \
__fun( 25, 3 ) \
__fun( 26, 0 ) \
__fun( 27, 0 ) \
__fun( 28, 0 ) \
__fun( 28, 1 ) \
__fun( 30, 0 ) \
__fun( 31, 0 )

#define __glue_builtin_name2(f,a1,a2) __builtin_##f##_##a1##_##a2
#define __glue_builtin_name(f,a1,a2) __glue_builtin_name2(f,a1,a2)
#define __gen_mfc0(rn,sel) static inline uint32_t __attribute__((nomips16)) __glue_builtin_name(mfc0,rn,sel) () {  uint32_t v; asm volatile( "mfc0 %0,$" #rn "," #sel : "=r"(v) );  return v;  }
#define __gen_mtc0(rn,sel) static inline void     __attribute__((nomips16)) __glue_builtin_name(mtc0,rn,sel) ( uint32_t v ) {  asm volatile( "mtc0 %0,$" #rn "," #sel :: "r"(v) );  }
__gen_fset( __gen_mfc0 )

#define __builtin_mfc0(rn,sel)   __builtin_mfc0_##rn##_##sel()
#define __builtin_mtc0(rn,sel,v) __builtin_mtc0_##rn##_##sel(v)
__gen_fset( __gen_mtc0 )

#define __gen_bcc0(rn,sel) \
static inline void __attribute__((nomips16)) __glue_builtin_name(bcc0,rn,sel) ( uint32_t clr ) \
{ \
  uint32_t tmp1,tmp2; \
  asm volatile( \
    "mfc0    %[tmp1],$" #rn "," #sel    "\n"  \
    "move    %[tmp2],%[clr]"            "\n"  \
    "not     %[tmp2],%[tmp2]"           "\n"  \
    "and     %[tmp1],%[tmp1],%[tmp2]"   "\n"  \
    "mtc0    %[tmp1],$" #rn "," #sel    "\n"  \
    : [clr] "=r"(clr), [tmp1] "=r"(tmp1), [tmp2] "=r"(tmp2) );     \
}
#define __builtin_bcc0(rn,sel,clr) __builtin_bcc0_##rn##_##sel(clr)
__gen_fset( __gen_bcc0 )

#define __gen_bsc0(rn,sel) \
static inline void __attribute__((nomips16)) __glue_builtin_name(bsc0,rn,sel) ( uint32_t v ) \
{ \
  uint32_t tmp; \
  asm volatile( \
    "mfc0    %[t],$" #rn "," #sel "\n"  \
    "or      %[t],%[t],%[v]"      "\n"  \
    "mtc0    %[t],$" #rn "," #sel "\n"  \
    : [v] "=r"(v), [t] "=r"(tmp) );     \
}
#define __builtin_bsc0(rn,sel,v) __builtin_bsc0_##rn##_##sel(v)
__gen_fset( __gen_bsc0 )

#define __gen_bcsc0(rn,sel) \
static inline void __attribute__((nomips16)) __glue_builtin_name(bcsc0,rn,sel) ( uint32_t clr, uint32_t set ) \
{ \
  uint32_t tmp1,tmp2; \
  asm volatile( \
    "mfc0    %[tmp1],$" #rn "," #sel    "\n"  \
    "move    %[tmp2],%[clr]"            "\n"  \
    "not     %[tmp2],%[tmp2]"           "\n"  \
    "and     %[tmp1],%[tmp1],%[tmp2]"   "\n"  \
    "or      %[tmp1],%[tmp1],%[set]"    "\n"  \
    "mtc0    %[tmp1],$" #rn "," #sel    "\n"  \
    : [set] "=r"(set), [clr] "=r"(clr), [tmp1] "=r"(tmp1), [tmp2] "=r"(tmp2) );     \
}
#define __builtin_bcsc0(rn,sel,clr,set) __builtin_bcsc0_##rn##_##sel(clr,set)
__gen_fset( __gen_bcsc0 )

#define _mfc0(rn, sel)            __builtin_mfc0(rn,sel)
#define _mtc0(rn, sel, v)         __builtin_mtc0(rn,sel,v)
// #define _mxc0(rn, sel, v)         __builtin_mxc0(rn,sel,v)         // exchange (swap) VAL and CP0 register REG,SEL
#define _bcc0(rn, sel, clr)       __builtin_bcc0(rn,sel,clr)       // bit clear non-zero bits from CLR in CP0 register REG,SEL
#define _bsc0(rn, sel, set)       __builtin_bsc0(rn,sel,set)       // bit set non-zero bits from SET in CP0 register REG,SEL
#define _bcsc0(rn, sel, clr, set) __builtin_bcsc0(rn,sel,clr,set)  // bit clear non-zero bits from CLR and set non-zero bits from SET in REG,SEL

// count leading zeroes
#define _clz(x) __builtin_clz (x)

// count trailing zeroes
#define _ctz(x) __builtin_ctz (x)

#endif

#include <cp0defs.h>

#define __ISRz(n) __vector_##n
#define __ISRname(n) __ISRz(n)
#define __ISRsoft(y)    void __attribute__((interrupt,nomicromips,nomips16)) __ISRname(y)()
#define __ISRshadow(y)  void __attribute__((interrupt,use_shadow_register_set,nomicromips,nomips16)) __ISRname(y)()

#endif
