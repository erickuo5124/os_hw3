[![hackmd-github-sync-badge](https://hackmd.io/fAE43hbVS6KUPn0L7c3slA/badge)](https://hackmd.io/fAE43hbVS6KUPn0L7c3slA)
# 2020_OS_Fall_HW3: Key-Value Stroages

### 基本資訊
**學號：f74072269**

**姓名：郭又宗**

**系級：資訊系 111 級**

---

## 開發環境
- OS : Ubuntu 20.04.1 LTS
- CPU : Intel(R) Core(TM) i7-8550U CPU @ 1.80GHz
- Memory : ~~20GB~~ -> 4GB
- Programming Language : gcc (Ubuntu 9.3.0-10ubuntu2) 9.3.0, python 3.8.5

---

## 程式執行時間
![](https://i.imgur.com/IZ6pvl0.png)

---

## 程式開發與使用說明
### 使用說明
```bash=
$ make
$ ./main "file/path"
```

### 程式開發
本次作業的重點在於如何**存放**以及**整理**資料，因為 key 會在一個很大的範圍內，單純的陣列或是使用一個檔案來儲存是有點困難的。

我的第一個想法是宣告很多陣列，並將陣列存到 *stroage* 資料夾中。因此我將 key 分成兩個值，**前面 48 bits 跟後面 16 bits**

- 前 48 bits : 第幾個陣列
- 後 16 bits : 資料在陣列中的索引值

而將陣列存成檔案我第一個想到的是 python 的 numpy 這個套件，因此決定從 python 下手

寫完確定正確了之後用大概 5M 的檔案測試，結果如下...

![](https://i.imgur.com/pyF9vtF.png)

因為所有 key 值都是隨機的，所以 SCAN 會跑很久，因此就不測試 small3.input 了

然後我照這個邏輯寫了 C 語言版本的 key-value stroage，但跟上面問題一樣，可能是生成 key 值太隨機，每個差異太大，導致要不斷開新檔案來儲存。導致在我跑 4G 的檔案時(全部都是 PUT)，跑了快半小時還沒跑完，於是我決定將程式終止，並且將暫存檔刪除。在我執行 `rm -rf ./stroage` 時 ubuntu 開始卡頓，我決定用 magin sysrq 強制關機，然後我就再也打不開 ubuntu 了...

轉換到 windows 之後，因為好像有中毒的關係，所以就連測試資料也要跑很久。最後轉換到虛擬機上面

*--轉換到虛擬機上--*
![](https://i.imgur.com/Ff3pdfN.png)

就算是在虛擬機上，跑同樣的小檔案也明顯快了許多，但大檔案可能還要再考慮一下怎麼處理會比較好

---

## 分析報告
![](https://i.imgur.com/8T4E3Mo.png)

![](https://i.imgur.com/i7ASyV4.png)


儲存資料方式是用類似 paging 的概念，把前面幾個 bit 拿來當 page 的 index，後面的 bit 拿來當 page 中的 offset，如此的好處是資料不需要排序，會自己存到相對應的位置，取出也很方便。但缺點是如果資料的範圍大且分散，就要開很多檔案來存放資料。

從第二張圖可以看出來這次程式幾乎都在寫檔，而內容應該大多是在做 SCAN 的工作，因為我產生的資料真的是太隨機了。讀檔的話是在一瞬間就完成的事情，記憶體占用則沒有很多，除非說有某些數值會常常出現，這樣可能可以設計一個 "cache" 來存資料，增加程式運行速度。