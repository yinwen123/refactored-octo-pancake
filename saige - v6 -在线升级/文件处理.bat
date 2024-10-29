@echo off
::0    BIN0路径  %~dp0
::1    输出路径
::2    输出文件类型
::3    full文件格式
::4    输出源文件名称
::5    输出文件名称1
::6    输出文件名称2
::7    扩展   是否添加校验YN   /   版本名称.date
::8    用户数据地址   0 / 1 / ……
::9    用户数据   0 / 1 / ……
::10   起始地址 BIN  0 / 1 /  2……
::11   BIN1文件路径 
::12   BIN2文件路径 
::13   ……

set str1=Out2K

set str2=.cwb/.full

set str3=单线升级10K

set str4=YD_YS11_10
set str5=雅迪YS11_10
set str6=雅迪YS11_10

set str7=Y/ver

set str8=4FE0
set str9=YD_NFC_YB_BXXXXXAA55

set str10=0800/0

set str11=D:\BOOT.bin

set str12=0

if exist %str1% del /f /q %str1%

if not exist %str1% md %str1%

start ANDFILE1.exe %~dp0 %str1% %str2% %str3% %str4% %str5% %str6% %str7% %str8% %str9% %str10% %str11% %str12%