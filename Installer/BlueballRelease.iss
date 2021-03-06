; Script generated by the Inno Setup Script Wizard.
; SEE THE DOCUMENTATION FOR DETAILS ON CREATING INNO SETUP SCRIPT FILES!

#define MyAppName "BlueBall"
#define MyAppVersion "0.01"
#define MyAppPublisher "Rafflesia"
#define MyAppURL "https://www.example.com/"
#define MyAppExeName "GTXDankEngine.exe"
#define CPlusPlusYearVersion '2019'


[Setup] 
DisableWelcomePage=no
WizardImageFile=Installer\INSTALLERFILES\panel_image_100.bmp
WizardSmallImageFile=Installer\INSTALLERFILES\banner.bmp
; NOTE: The value of AppId uniquely identifies this application. Do not use the same AppId value in installers for other applications.
; (To generate a new GUID, click Tools | Generate GUID inside the IDE.)
AppId={{236E21AE-04B8-4C03-985B-04F4526BADE6}
AppName={#MyAppName}
AppVersion={#MyAppVersion}
;AppVerName={#MyAppName} {#MyAppVersion}
AppPublisher=DigiPen Institute of Technology
AppPublisherURL=http://www.digipen.edu/
AppSupportURL=http://www.digipen.edu/
;AppUpdatesURL={#MyAppURL}
DefaultDirName={userdocs}\Digipen\{#MyAppName}
DefaultGroupName=DigiPen\{#MyAppName}
;DisableProgramGroupPage=yes
; Uncomment the following line to run in non administrative install mode (install for current user only.)
;PrivilegesRequired=lowest 
SourceDir=..\
OutputDir=Installer\INSTALLER
OutputBaseFilename={#MyAppName}_Setup2

; Path to the DigiPen EULA (Needed to pass TCRs)
LicenseFile=Installer\INSTALLERFILES\DigiPen_EULA.txt

Compression=lzma
SolidCompression=yes
WizardStyle=modern

; Path to the icon for the installer (TCR check requires custom icon)
SetupIconFile=Installer\INSTALLERFILES\setup_icon.ico

; This allows the installer to run without admin privileges, which means you can't install
; in the Program Files, or change the registry. This is done to allow installation on Sandbox
; or other intermediate directory
PrivilegesRequired=none
RestartIfNeededByRun=no

[Languages]
Name: "english"; MessagesFile: "compiler:Default.isl"

[Tasks]
Name: "desktopicon"; Description: "{cm:CreateDesktopIcon}"; GroupDescription: "{cm:AdditionalIcons}"; Flags: unchecked

[Files]
Source: "GTXDankEngine\x64\Release\{#MyAppExeName}"; DestDir: "{app}\Platformer"; Flags: ignoreversion
Source: "*"; DestDir: "{app}"; Flags: ignoreversion
Source: "Platformer\*"; DestDir: "{app}\Platformer"; Flags: ignoreversion recursesubdirs createallsubdirs
Source: "GTXDankEngine\assimp-vc142-mtd.dll"; DestDir: "{app}\Platformer"; Flags: ignoreversion
Source: "GTXDankEngine\fmod.dll"; DestDir: "{app}\Platformer"; Flags: ignoreversion
Source: "GTXDankEngine\fmodstudio.dll"; DestDir: "{app}\Platformer"; Flags: ignoreversion
Source: "GTXDankEngine\freetype.dll"; DestDir: "{app}\Platformer"; Flags: ignoreversion
Source: "GTXDankEngine\Assets\*";  DestDir: "{app}\Platformer\Assets"; Flags: ignoreversion recursesubdirs createallsubdirs
Source: "GTXDankEngine\Assets\*";  DestDir: "{app}\GTXDankEngine\Assets"; Flags: ignoreversion recursesubdirs createallsubdirs
Source: "GTXDankEngine\Source\Shaders\*"; DestDir: "{app}\Platformer\Source\Shaders"; Flags: ignoreversion recursesubdirs createallsubdirs
Source: "Installer\REDIST\VC_redist.x64.exe"; DestDir: "{app}\Platformer"; Flags: ignoreversion deleteafterinstall
Source: "Installer\INSTALLERFILES\panel_image_*.bmp"; DestDir: "{app}\Platformer"; Flags: ignoreversion dontcopy
; NOTE: Don't use "Flags: ignoreversion" on any shared system files

[Icons]
Name: "{autoprograms}\{#MyAppName}"; Filename: "{app}\Platformer\{#MyAppExeName}"
Name: "{autodesktop}\{#MyAppName}"; Filename: "{app}\Platformer\{#MyAppExeName}"; Tasks: desktopicon

[Run]
Filename: "{app}\Platformer\VC_redist.x64.exe";    Parameters: /q /quiet /passive /verysilent /norestart /noreboot; Description: "{cm:LaunchProgram,{#StringChange(MyAppName, '&', '&&')}}"; Flags:   ;StatusMsg: Installing Visual C++ {#CPlusPlusYearVersion} Redistributable...
Filename: "{app}\Platformer\{#MyAppExeName}"; Description: "{cm:LaunchProgram,{#StringChange(MyAppName, '&', '&&')}}"; Flags: nowait postinstall skipifsilent

