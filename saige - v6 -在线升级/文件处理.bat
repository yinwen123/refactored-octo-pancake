@echo off
::0    BIN0·��  %~dp0
::1    ���·��
::2    ����ļ�����
::3    full�ļ���ʽ
::4    ���Դ�ļ�����
::5    ����ļ�����1
::6    ����ļ�����2
::7    ��չ   �Ƿ����У��YN   /   �汾����.date
::8    �û����ݵ�ַ   0 / 1 / ����
::9    �û�����   0 / 1 / ����
::10   ��ʼ��ַ BIN  0 / 1 /  2����
::11   BIN1�ļ�·�� 
::12   BIN2�ļ�·�� 
::13   ����

set str1=Out2K

set str2=.cwb/.full

set str3=��������10K

set str4=YD_YS11_10
set str5=�ŵ�YS11_10
set str6=�ŵ�YS11_10

set str7=Y/ver

set str8=4FE0
set str9=YD_NFC_YB_BXXXXXAA55

set str10=0800/0

set str11=D:\BOOT.bin

set str12=0

if exist %str1% del /f /q %str1%

if not exist %str1% md %str1%

start ANDFILE1.exe %~dp0 %str1% %str2% %str3% %str4% %str5% %str6% %str7% %str8% %str9% %str10% %str11% %str12%