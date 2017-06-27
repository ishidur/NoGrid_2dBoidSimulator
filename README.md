# 2dBoidSimulator
Visual Studio/OpenGL/2D Boid Simulator
========================================================================

#### 使用したライブラリ
描画用に[freeglut](http://freeglut.sourceforge.net/ "freeglut")，ベクトル計算に[Eigen](http://eigen.tuxfamily.org/ "Eigen")を使用している．

#### Boidの動きについて
Boidの進行方向を決める要素としては5つがあリ，

・周りと同じ方向に進もうとする要素

・群れに向かって集まろうとする要素

・周りのBoidと一定の距離を保とうとする要素

・壁や障害物を回避しようとする要素

・自身の進行方向を保とうとする要素

以上の要素の足し合わせによって，進行方向が決定される．

#### 各クラスについて
本プログラムにはBoid，Direction，Block，Gridのクラスが存在している．
Directionクラスについては計算を簡略化させるためのツールである．
Boidクラスは，Boidに関する情報を持ったクラスである．Boidには，固有のid，現在の座標，進行する角度，スピード，所属するGridの番地，色の情報が保存される．
Blockクラスは，障害物に関する情報を持ったクラスである．このクラスには座標と半径，色の情報が保存される．
Gridクラスは，表示領域を分割したときの一区画の情報を保存するためのクラスである．Gridクラスには区画の位置，その区画内にいるBoidのid，その区画内にあるBlockのidの情報が保存される．

以上の4つのクラスを基に本プログラムは構成されている．

#### プログラム全体の流れについて
プログラムの全体としての動きとしては，以下のようになる．
1.	表示領域にGridを作成，配置
2.	Boidを配置し，GridにBoidのidを登録する
3.	Boid，Blockを描画する
4.	BoidのいるGridの情報からその周辺を含む9つのGridにいる他のBoidとBlockのidを取得する
5.	取得した近くにいる他のBoidとBlockの位置を基に，Boidの速度ベクトルを上記の規則に従って更新する
6.	すべてのBoidに対して，4.，5.より速度ベクトルの更新を行う
7.	速度ベクトルに応じてすべてのBoidの座標を更新する
8.	Grid内のBoidの情報を更新する
9.	3.~8.を繰り返す  
Blockが配置されると，GridにBlockのidを登録する
