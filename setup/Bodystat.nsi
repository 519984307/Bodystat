; �ýű�ʹ�� HM VNISEdit �ű��༭���򵼲���

; ��װ�����ʼ���峣��
!define PRODUCT_NAME "Bodystat"
!define PRODUCT_VERSION "1.0.0.1"
!define PRODUCT_PUBLISHER "yameng_he"
!define PRODUCT_UNINST_KEY "Software\Microsoft\Windows\CurrentVersion\Uninstall\${PRODUCT_NAME}"
!define PRODUCT_UNINST_ROOT_KEY "HKLM"

SetCompressor lzma

; ------ MUI �ִ����涨�� (1.67 �汾���ϼ���) ------
!include "MUI.nsh"

; MUI Ԥ���峣��
!define MUI_ABORTWARNING
!define MUI_ICON "${NSISDIR}\Contrib\Graphics\Icons\modern-install.ico"
!define MUI_UNICON "${NSISDIR}\Contrib\Graphics\Icons\modern-uninstall.ico"

; ��ӭҳ��
!insertmacro MUI_PAGE_WELCOME
; ��װĿ¼ѡ��ҳ��
!insertmacro MUI_PAGE_DIRECTORY
; ���Э��ҳ��
;!insertmacro MUI_PAGE_LICENSE "..\..\..\path\to\licence\YourSoftwareLicence.txt"
; ��װ����ҳ��
!insertmacro MUI_PAGE_INSTFILES
; ��װ���ҳ��
!insertmacro MUI_PAGE_FINISH

; ��װж�ع���ҳ��
!insertmacro MUI_UNPAGE_INSTFILES

; ��װ�����������������
!insertmacro MUI_LANGUAGE "SimpChinese"

; ��װԤ�ͷ��ļ�
!insertmacro MUI_RESERVEFILE_INSTALLOPTIONS
; ------ MUI �ִ����涨����� ------

Name "${PRODUCT_NAME} ${PRODUCT_VERSION}"
OutFile "Setup.exe"
InstallDir "D:\Bodystat"
ShowInstDetails show
ShowUnInstDetails show

Section "MainSection" SEC01
  SetOutPath "$INSTDIR"
  SetOverwrite ifnewer
  File /r "bin\*.*"
SectionEnd

Section -AdditionalIcons
  CreateDirectory "$SMPROGRAMS\Bodystat"
  CreateShortCut "$SMPROGRAMS\Bodystat\Uninstall.lnk" "$INSTDIR\uninst.exe"
  CreateShortCut "$DESKTOP\Bodystat.lnk" "$INSTDIR\Bodystat.exe"
SectionEnd

Section -Post
  WriteUninstaller "$INSTDIR\uninst.exe"
  WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "DisplayName" "$(^Name)"
  WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "UninstallString" "$INSTDIR\uninst.exe"
  WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "DisplayVersion" "${PRODUCT_VERSION}"
  WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "Publisher" "${PRODUCT_PUBLISHER}"
SectionEnd


Section Install
   System::Call 'msi::MsiQueryProductState(t"{6B66663C-055F-3A2E-A09D-168840A82362}")i.R0'
   IntCmp $R0 5 INSTALLSTATE_DEFAULT STATE_OTHER
	 STATE_OTHER:
	 IntCmp $R0 1 INSTALLSTATE_ADVERTISED INSTALLSTATE_ABSENT INSTALLSTATE_UNKNOWN
	 INSTALLSTATE_UNKNOWN:
  	DetailPrint "��װ״̬���ò�Ʒδ������װ"
  	ExecWait '"$INSTDIR\vc_redist.x64.exe" /q'
    Goto Done
	 INSTALLSTATE_ADVERTISED:
    DetailPrint "��װ״̬���ò�Ʒ�ѹ�浫δ��װ"
    ExecWait '"$INSTDIR\vc_redist.x64.exe" /q'
    Goto Done
	 INSTALLSTATE_ABSENT:
    DetailPrint "��װ״̬���ò�ƷΪ��ͬ���û���װ"
    ExecWait '"$INSTDIR\vc_redist.x64.exe" /q'
    Goto Done
	 INSTALLSTATE_DEFAULT:
    DetailPrint "��װ״̬���ò�Ʒ��Ϊ��ǰ�û���װ"
  done:
SectionEnd

/******************************
 *  �����ǰ�װ�����ж�ز���  *
 ******************************/

Section Uninstall
  Delete "$INSTDIR\uninst.exe"

  Delete "$SMPROGRAMS\Bodystat\Uninstall.lnk"
  
  Delete "$DESKTOP\Bodystat.lnk"

  RMDir "$SMPROGRAMS\Bodystat"

  RMDir /r "$INSTDIR\temp"
  RMDir /r "$INSTDIR\sqldrivers"
  RMDir /r "$INSTDIR\report"
  RMDir /r "$INSTDIR\platforms"

  RMDir /r "$INSTDIR"

  DeleteRegKey ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}"
  SetAutoClose true
SectionEnd

#-- ���� NSIS �ű��༭�������� Function ���α�������� Section ����֮���д���Ա��ⰲװ�������δ��Ԥ֪�����⡣--#

Function un.onInit
  MessageBox MB_ICONQUESTION|MB_YESNO|MB_DEFBUTTON2 "��ȷʵҪ��ȫ�Ƴ� $(^Name) ���������е������" IDYES +2
  Abort
FunctionEnd

Function un.onUninstSuccess
  HideWindow
  MessageBox MB_ICONINFORMATION|MB_OK "$(^Name) �ѳɹ��ش����ļ�����Ƴ���"
FunctionEnd
