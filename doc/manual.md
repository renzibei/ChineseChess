# ChineseChess 中国象棋 使用说明



1. 简要说明

   此软件采取端到端的去中心化网络连接，一方新建游戏等待连接，一方输入对方ip连接，建立游戏。

2. 简单规则说明

   游戏遵循基本象棋规则。

   - 不能对将，移动导致对将者判负。
   - 每回合限制思考时间60s,超时判负。

3. 功能介绍

   - 进行联机象棋游戏功能
   - 保存棋盘功能，生成存档文本文件
   - 读取棋盘存档功能，可以用存档建立新比赛
   - 走棋音效和将军音效
   - 可以认输

4. 程序架构介绍

   1. 采取OOP设计思想，运用单例模式、委托模式等设计模式。

   2. 主要的类有ChessPiece, ChessBoard, GameCenter, NetServer, GraphicsScene, PieceItem, MainWindow等。其中ChessPiece是棋子类，储存有棋子的数据，ChessBoard为棋盘类，有棋盘信息，并充当棋子工厂，GameCenter为后台数据中心，储存有游戏信息并管理，NetServer为网络服务委托类，可以通过其收发数据，GraphicsScene为继承了QGraphicsScene的场景类，管理棋子、棋盘的视图、屏幕位置、鼠标交互操作等。PieceItem为棋子的视图Item类。

   3. 工作流程

      1. 在主程序的菜单栏选择新建游戏，NetServer开始监听
      2. 另一客户端加入游戏，建立连接，双方的GameCenter开始运转
      3. 交互过程中双方传输数据，通过NetServer并用GameCenter处理、同步。

   4. 数据交换格式

      先用网络字节序传送4个字节的整形表示包长度，然后传输json包装的信息。包内容详情见`doc/jsonFormat.md`.

5. 编译安装

   见readme.md