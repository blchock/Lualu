; Script generated by the Inno Setup Script Wizard.
; SEE THE DOCUMENTATION FOR DETAILS ON CREATING INNO SETUP SCRIPT FILES!

[Setup]
; NOTE: The value of AppId uniquely identifies this application.
; Do not use the same AppId value in installers for other applications.
; (To generate a new GUID, click Tools | Generate GUID inside the IDE.)
AppId={{5C42B4CB-996B-4ED7-8E94-264E82C7F1F2}
AppName=Lualu
AppVersion=1.0.0
AppPublisher=Bl.Chock
AppPublisherURL=https://github.com/blchock/Lualu
AppSupportURL=https://github.com/blchock/Lualu
AppUpdatesURL=https://github.com/blchock/Lualu
DefaultDirName={pf}\LuaLu
DisableProgramGroupPage=yes
OutputBaseFilename=Lualu
SetupIconFile=LuaLu\LuaLu.ico
Compression=lzma
SolidCompression=yes
; ArchitecturesInstallIn64BitMode=x64

[Languages]
Name: "english"; MessagesFile: "compiler:Default.isl"

[Tasks]
Name: "desktopicon"; Description: "{cm:CreateDesktopIcon}"; GroupDescription: "{cm:AdditionalIcons}"; Flags: unchecked

[Files]
Source: "Release\LuaLu.exe"; DestDir: "{app}"; Flags: ignoreversion
Source: "Release\*"; DestDir: "{app}"; Excludes: "*.iobj,*.ipdb,*.pdb"; Flags: ignoreversion recursesubdirs createallsubdirs
; NOTE: Don't use "Flags: ignoreversion" on any shared system files

[Icons]
Name: "{commonprograms}\Lualu"; Filename: "{app}\LuaLu.exe"
Name: "{commondesktop}\Lualu"; Filename: "{app}\LuaLu.exe"; Tasks: desktopicon

[Run]
Filename: "{app}\LuaLu.exe"; Description: "{cm:LaunchProgram,VirtualWorld}"; Flags: shellexec postinstall skipifsilent

