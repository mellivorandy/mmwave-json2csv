# mmwave-json2csv

<br>

[<img alt="ci" src="https://github.com/mellivorandy/mmwave-json2csv/actions/workflows/c-cpp.yml/badge.svg" height="20">](https://github.com/mellivorandy/mmwave-json2csv/actions)
[<img alt="license" src="https://img.shields.io/github/license/mellivorandy/mmwave-json2csv?style=for-the-badge&logo=GITHUB&color=light%20green" height="20">](https://github.com/mellivorandy/mmwave-json2csv?tab=MIT-1-ov-file)

<br>

將 TI mmWave Sensor `IWRL6432BOOST` EVM 感測資料的 JSON 格式自動轉換為 CSV，方便資料分析與後續處理。

<br>

## 特色 Features

- **JSON → CSV 轉換**：支援 IWRL6432BOOST EVM 的 json 格式。

- **自訂欄位**：可根據實際資料選擇需轉換的內容。

- **跨平台**：可在 Windows (MSYS2/MinGW64)、Linux (g++) 下編譯執行。

- **快速批次處理**：適合大量 mmWave 資料處理。

<br>

## 使用方法 Usage

### 1. 環境設定

#### Windows (MSYS2 MinGW64)
```bash
pacman -S mingw-w64-x86_64-gcc make
```

<br>

#### Linux & macOS
Linux (Debian/Ubuntu)

```bash
sudo apt install g++ make
```

macOS (Homebrew)

```bash
brew install gcc make
```

<br>

### 2. 編譯 & 執行

#### Clone 此專案
```bash
git clone https://github.com/mellivorandy/mmwave-json2csv.git
```

<br>

#### 編譯
進入 `src/` 目錄
```bash
cd src/
```

```bash
make
```

<br>

#### 執行
```bash
./json2csv.exe
```

或

```bash
./json2csv
```

<br>

## 輸入/輸出格式

- **輸入**：TI mmWave Sensor `IWRL6432BOOST` EVM 的 JSON 格式資料檔案

- **輸出**：標準 CSV 檔案，欄位根據感測資料自動產生

<br>

## 應用 Applications

- mmWave 感測資料處理

- 資料科學分析前格式轉換

- 機器學習訓練資料準備

<br>

## 貢獻 Contributing

歡迎 issue、PR 或討論！  
如需自訂欄位、支援其他 JSON 格式，請開 GitHub Issues。

<br>

## VSCode 終端機設置教學（MSYS2/MinGW64）

在 Windows VSCode 內預設終端機為 PowerShell，如果要使用 `make`，需要將預設終端機改成 MinGW64 環境才能編譯 C++（如 `make`、`g++`）。

<br>

### 步驟一：確認你已安裝 MSYS2

- 如果沒裝，請先參考 [MSYS2 官方網站](https://www.msys2.org/) 安裝。

- 安裝後，確定可以在 Windows 搜尋選單看到 `MSYS2 MinGW 64-bit`。

<br>

### 步驟二：在 VSCode 新增 MinGW64 終端機 Profile

1. **打開 VSCode**

2. 按下 `Ctrl + Shift + P` 打開搜尋面板

3. 輸入並選擇：`Terminal: Select Default Profile`

4. 選擇 bash (MSYS2) 作為預設終端機 (之後有需要可以再改回來)

5. 按下 `Ctrl + ,`，並搜尋 `Terminal Integrated Automation Profile: Windows`
    
6. 點選 Edit in settings.json

7. 在原本的 "terminal.integrated.profiles.windows" 下的 "bash (MSYS2)" 之 "env" 欄位加入 `"MSYSTEM": "MINGW64",`

<br>

加入後會長這樣

```json
"PowerShell": {
    "_comment": "Omitted here."
},
"Command Prompt": {
    "_comment": "Omitted here."       
},
"Git Bash": {
    "_comment": "Omitted here."
},
"bash (MSYS2)": {
    "path": "C:\\msys64\\usr\\bin\\bash.exe",
    "args": [
        "--login",
        "-i"
    ],
    "env": {
        "MSYSTEM": "MINGW64",
        "CHERE_INVOKING": "1"
    }
}
```

<br>

8. 接著在與 `"terminal.integrated.profiles.windows"` 同一層下方加入

```json
"terminal.integrated.profiles.windows": {
    "_comment": "Omitted here."
},
"terminal.integrated.env.windows": {
    "CHERE_INVOKING": "1",
    "MSYSTEM": "MINGW64"
}
```

<br>

9. 最終長這樣 (json 其中一部分)

```json
"terminal.integrated.defaultProfile.windows": "bash (MSYS2)",
"terminal.integrated.profiles.windows": {
    "PowerShell": {
        "source": "PowerShell",
        "icon": "terminal-powershell"
    },
    "Command Prompt": {
        "path": [
            "${env:windir}\\Sysnative\\cmd.exe",
            "${env:windir}\\System32\\cmd.exe"
        ],
        "args": [],
        "icon": "terminal-cmd"
    },
    "Git Bash": {
        "source": "Git Bash"
    },
    "bash (MSYS2)": {
        "path": "C:\\msys64\\usr\\bin\\bash.exe",
        "args": [
            "--login",
            "-i"
        ],
        "env": {
            "MSYSTEM": "MINGW64",
            "CHERE_INVOKING": "1"
        }
    }
},
"terminal.integrated.automationProfile.windows": {},
"terminal.integrated.env.windows": {
    "CHERE_INVOKING": "1",
    "MSYSTEM": "MINGW64"
}
```

<br>

> 請依你的 MSYS2 實際安裝路徑調整。

<br>

### 步驟三：啟用 MINGW64 終端機

- 點選 VSCode 下方「終端機」區域的下拉選單 (加號旁邊箭頭)，選擇你剛剛新增的 `bash (MSYS2)` 終端機。

- 開啟後，終端機 prompt 會出現類似這樣：
```bash
MINGW64
$
```

- 在這個終端機裡，`g++`、`make`、Unix/Linux 指令都能用。

<br>

### 步驟四：測試編譯指令

在 bash (MSYS2) 終端機中輸入：

```bash
g++ --version
make --version
```

如果都顯示出版本資訊，代表設置成功！若沒有顯示，請執行 [環境設定](#1-環境設定) 中的指令。

<br>

### 常見問題
如果 VSCode 終端機還是找不到 `g++`、`make`，請確認你選的是 `MINGW64` 不是 `MSYS` 或 `PowerShell`。

<br>

**完成設置後，編譯、執行 C++ 專案都能在 VSCode 終端機直接進行！**

如有任何疑問，歡迎留言或開 issues。

<br>

## License

The source code is licensed under <a href="LICENSE">MIT license</a>.

<br>

---

**English Summary**

mmwave-json2csv is a simple command-line tool that converts mmWave sensor JSON data to CSV format for easier analysis and post-processing.

- Build: `make`
- Run: `./json2csv`
- Features: Fast conversion, customizable fields, cross-platform (Windows/Linux)

Feel free to open issues or PRs for improvements!
