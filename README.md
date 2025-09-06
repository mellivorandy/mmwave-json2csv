# mmwave-json2csv

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

#### 編譯
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
