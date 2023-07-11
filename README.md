# i1i2i3

## アイデア

```
・マルチスレッドを使わずに実装

・FFTを使ってデータ量を削減 & 雑音除去

・周波数を高くして出力する変声

・自分の声のデータを保持しておいてそれに類似する音の出力をカット

・機械学習モデルを使って文字起こし

・UDPを使って実装

・多者間通話

・ミュート機能
```

## while を使った phone
```
Nの値を16384と大きめに設定することで、recv関数以下の処理が終わる前に同じ週にsend関数で送ったデータを再生し切ってしまい途切れ途切れになるのを防いだ。
同時にバッファのオーバーランも防げた。
```

# vscode で github のコードを編集する方法

### - githubのファイルをカレントディレクトリにインポート
```
git clone "https://github.com/Riku0413/i1i2i3.git"
```

### - 編集したファイルをステージに上げる
```
git add filename
```

### - 編集した全てのファイルをステージに上げる
```
git add -A
```

### - 編集内容を説明に加える
```
git commit -m "ここに説明を加える"
```

### - 編集したファイル情報をgithubにプッシュ
```
git push origin main
```

### - 現在の編集状況を確認
```
git status
```

# 実行方法

### - まずは以下のコードによりコンパイル
```
gcc i1i2i3_phone_0710_while_fft.c fft.c -o i1i2i3_phone_0710_while_fft
```

### - 通信開始時はサーバ側とクライアント側が以下をそれぞれ実行する

#### ・サーバ側
```
./i1i2i3_phone_0710_while_fft <ポート番号>
```

#### ・クライアント側
```
./i1i2i3_phone_0710_while_fft <サーバのIPアドレス> <ポート番号>
```
gcc i1i2i3_phone_0710_while_fft.c fft.c -o i1i2i3_phone_0710_while_fft
#### 臨時！！！
```
gcc i1i2i3_phone_0711_while_fft.c fft_0711.c -o i1i2i3_phone_0711_while_fft
```