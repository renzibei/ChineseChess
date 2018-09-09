# jsonFormat

开头四个字节为包长度

后面为json数据

json构成为 {"packageType"：number ...}

***

packageType = 0

反馈包

result = 0代表正常，1代表出错

{"packageType", "result":number}

***

packageType = 1

走棋包，{"packageType" , "from":{"x":number, "y":number}, "to":{x, y}}

***

2

将军losType为1，认输loseType为2, 超时loseType为3, 对将为4

输包{"packageType", "loseType":number



***

3

新游戏

gameMode 为0时代表新游戏，为1时代表残局模式，并传送残局地图

{"packageType",  "gameMode":number, hostColor":bool, "map":string}

