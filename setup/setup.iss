; Script generated by the Inno Setup Script Wizard.
; SEE THE DOCUMENTATION FOR DETAILS ON CREATING INNO SETUP SCRIPT FILES!

#define MyAppName "GTester"
#define MyAppVersion "0.1"
#define MyAppExeName "GTester.exe"

[Setup]
; NOTE: The value of AppId uniquely identifies this application.
; Do not use the same AppId value in installers for other applications.
; (To generate a new GUID, click Tools | Generate GUID inside the IDE.)
AppId={{57E01954-7659-400B-9B78-96BEB2271C97}
AppName={#MyAppName}
AppVersion={#MyAppVersion}
;AppVerName={#MyAppName} {#MyAppVersion}
DefaultDirName={pf64}\{#MyAppName}
DefaultGroupName={#MyAppName}
AllowNoIcons=yes
LicenseFile=D:\Downloads\gpl-3.0.txt
OutputBaseFilename=gtester-setup
Compression=lzma
SolidCompression=yes
ArchitecturesAllowed=x64

[Languages]
Name: "english"; MessagesFile: "compiler:Default.isl"

[Tasks]
Name: "desktopicon"; Description: "{cm:CreateDesktopIcon}"; GroupDescription: "{cm:AdditionalIcons}"; Flags: unchecked

[Files]
Source: "..\..\build-GTester-Desktop_Qt_5_2_0_MSVC2012_OpenGL_64bit-Release\release\GTester.exe"; DestDir: "{app}"; Flags: ignoreversion
; NOTE: Don't use "Flags: ignoreversion" on any shared system files
Source: "C:\Qt\Qt5.2.0\5.2.0\msvc2012_64_opengl\bin\Qt5Core.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "C:\Qt\Qt5.2.0\5.2.0\msvc2012_64_opengl\bin\Qt5Gui.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "C:\Qt\Qt5.2.0\5.2.0\msvc2012_64_opengl\bin\Qt5Widgets.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "c:\Qt\Qt5.2.0\5.2.0\msvc2012_64_opengl\plugins\platforms\qwindows.dll"; DestDir: "{app}\platforms"; Flags: ignoreversion
Source: "C:\Qt\Qt5.2.0\5.2.0\msvc2012_64_opengl\bin\icuin51.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "C:\Qt\Qt5.2.0\5.2.0\msvc2012_64_opengl\bin\icuuc51.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "C:\Qt\Qt5.2.0\5.2.0\msvc2012_64_opengl\bin\icudt51.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "C:\Windows\sysnative\msvcp110.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "C:\Windows\sysnative\msvcr110.dll"; DestDir: "{app}"; Flags: ignoreversion

[Icons]
Name: "{group}\{#MyAppName}"; Filename: "{app}\{#MyAppExeName}"
Name: "{commondesktop}\{#MyAppName}"; Filename: "{app}\{#MyAppExeName}"; Tasks: desktopicon

[Run]
Filename: "{app}\{#MyAppExeName}"; Description: "{cm:LaunchProgram,{#StringChange(MyAppName, '&', '&&')}}"; Flags: nowait postinstall skipifsilent