$wd = Get-Location

Write-Host "Step 1: Updating system..."
winget upgrade --all

Write-Host "Step 2: Installing necessary packages (Git, Python 3.11, LLVM)..."
winget install Git.Git
winget install Python.Python.3.11
winget install LLVM.LLVM

Write-Host "Step 3: Installing Python dependencies..."
$requirementsPath = "$wd\requirements.txt"
python -m pip install -r $requirementsPath

Write-Host "Step 4: Installing Rust..."
Invoke-WebRequest -Uri "https://sh.rustup.rs" -UseBasicParsing | Invoke-Expression

Write-Host "Step 5: Installing Visual Studio C++ Build Tools..."
winget install Microsoft.VisualStudio.2022.BuildTools

Write-Host "Step 6: Installing MSVC v143 and other necessary C++ components..."
$vsInstallerPath = "C:\Program Files (x86)\Microsoft Visual Studio\Installer\vs_installer.exe"
& $vsInstallerPath modify --installPath "C:\BuildTools" `
    --add Microsoft.VisualStudio.Component.VC.Tools.x86.x64 `
    --add Microsoft.VisualStudio.Component.VC.143.x86.x64 `
    --add Microsoft.VisualStudio.Component.Windows10SDK.19041 `
    --includeRecommended --passive

Write-Host "Step 7: Installing the library with Maturin..."
python -m pip install torch torchvision torchaudio --index-url https://download.pytorch.org/whl/cu121
python -m pip install maturin
python -m pip install ultralytics
python -m maturin build --release
python -m pip install target/wheels/* --force-reinstall
Write-Host "Installation and setup complete!"
