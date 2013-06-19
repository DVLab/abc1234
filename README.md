README
=======
使用方法 <br/>
------------------------------------------------------------<br/>
第一次修改： <br/>
1.git clone https://github.com/DVLab/abc1234.git <br/>
2.下載SoCV PA3的空殼 <br/>
3.將abc1234資料夾中的東西全部複製（包含.git資料夾）到pa3的資料夾,覆蓋掉原本的檔案 <br/>
4.修改程式碼 <br/>
5.git add src <br/> 
6.git commit -m "請簡述一下做了哪些更改,新增了什麼功能etc" <br/>
7.git push origin master <br/>
8.輸入帳號密碼 <br/> 
9.完成 <br/>
 <br/>
------------------------------------------------------------<br/>
第n次修改： <br/>
1.git pull origin <br/>
2.git會自動將程式碼跟github上的做merge,若有conflict請手動處理 <br/>
3.修改程式碼<br/>
4.git add src<br/>
5.git commit -m "請簡述一下做了哪些更改,新增了什麼功能etc"<br/>
6.git push origin master<br/>
7.輸入帳號密碼<br/>
8.完成<br/>
<br/>
------------------------------------------------------------<br/>
<br/>
ps:<br/>
1.根目錄請放.gitignore檔案,並請注意commit時不要加入.o檔或其他binary file<br/>
2.請勿直接於根目錄執行git add *以免將cscope或ctag之類的檔案一起commit出去,若有需要add其他檔案於根目錄,請於根目錄執行git add filename<br/>
3.目前先統一用master,若需要開其他的branch請於fb社團告知大家<br/>
4.若有其他問題,例如無法compile,請儘快到fb社團告知,謝謝<br/>
