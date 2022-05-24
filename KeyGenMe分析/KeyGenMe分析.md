###### 花指令即构造恒成立跳转如

xor eax,eax

je eax Label

//这里全是垃圾指令

Label:



### KeyGenMe分析

用OD手工去除花指令（把恒成立跳转中的代码全部nop掉防止干扰），然后保存出来新的exe文件

然后用ida分析

![image-20220524111337263](C:\Users\Admin\AppData\Roaming\Typora\typora-user-images\image-20220524111337263.png)



来到sub_401000



![image-20220524111424088](C:\Users\Admin\AppData\Roaming\Typora\typora-user-images\image-20220524111424088.png)

二重循环里即加密方法， 一个数组与输入的字符串每一位异或

推测真码为18位

发现第二个循环结果只取决于j=6的时候，寻找byte_442198[6]的值，为'n'



![image-20220524112049073](C:\Users\Admin\AppData\Roaming\Typora\typora-user-images\image-20220524112049073.png)



str1[i] = int(‘n') ^ (str[i] + 13)

则str[i] = str1[i]^int('n') - 13

编写程序



![image-20220524113245819](C:\Users\Admin\AppData\Roaming\Typora\typora-user-images\image-20220524113245819.png)

发现计算得出的字符串很有特征，应该为真码，于是扔到去除花指令的程序试了一下，发现失败，感觉很蹊跷，怀疑是修改花指令的时候出现问题，于是去原程序那里输入，发现提示注册成功



![image-20220524113522628](C:\Users\Admin\AppData\Roaming\Typora\typora-user-images\image-20220524113522628.png)





将去除花指令的程序重新打开OD分析一下

发现清花指令的时候好像“不小心”（蔡）把注册成功给清掉了。。。不过算法部分不受影响