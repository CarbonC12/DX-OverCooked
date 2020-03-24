#include "d3dUtility.h"
#include "Light.h"
#include "gameTimer.h"
#include<mmsystem.h>  //用于播放音效的头文件


int ViewTurn = 0; //用于转换视角
int menuFlag = 0; //画初始菜单
int flag = 0;  //画多个传送带
int DrawTime = 0;//防止多次创建顶点缓存占用空间
int MenuKind = 0;//指示现在是需要输出那一个菜品 
int IsOverCooked = 0;
int menunum = rand() % 100;
//预创建顶点缓存
D3D11_BUFFER_DESC bd;
HRESULT hr;
UINT vertexCount;
D3D11_SUBRESOURCE_DATA InitData;
ID3D11Buffer* vertexBuffer;
//声明全局的指针
ID3D11Device* device = NULL;//D3D11设备接口
IDXGISwapChain* swapChain = NULL;//交换链接口
ID3D11DeviceContext* immediateContext = NULL;
ID3D11RenderTargetView* renderTargetView = NULL;//渲染目标视图  

//Effect相关全局指针
ID3D11InputLayout* vertexLayout;
ID3DX11Effect* effect;
ID3DX11EffectTechnique* technique;

//声明三个坐标系矩阵
XMMATRIX world;         //用于世界变换的矩阵
XMMATRIX view;          //用于观察变换的矩阵
XMMATRIX projection;    //用于投影变换的矩阵

//声明材质和光照的全局对象
Material		boxMaterial;      //箱子材质
Material		floorMaterial;    //地板材质
Material		waterMaterial;    //水面材质
Light			light[3];      //光源数组
int             lightType = 0;  //光源类型

ID3D11Texture2D* depthStencilBuffer;
ID3D11DepthStencilView* depthStencilView;  //深度模板视图

ID3D11ShaderResourceView* textureBox;      //箱子纹理
ID3D11ShaderResourceView* textureFloor;    //地板纹理
ID3D11ShaderResourceView* textureDrawer;    //抽屉纹理
ID3D11ShaderResourceView* textureWall;    //墙面纹理
ID3D11ShaderResourceView* textureWallTop;    //墙面顶部纹理
ID3D11ShaderResourceView* textureScrollLine;    //传送带纹理
ID3D11ShaderResourceView* texturePlayer1;    //人物纹理
ID3D11ShaderResourceView* texturePlayer2;    //人物2纹理
ID3D11ShaderResourceView* textureVegetable;  //蔬菜纹理
ID3D11ShaderResourceView* texturePlateOfPalyer1;
ID3D11ShaderResourceView* textureBreadCutted;	//切好的面包
ID3D11ShaderResourceView* textureMeatCooked;
ID3D11ShaderResourceView* textureLettuceCutted;
ID3D11ShaderResourceView* textureTomatoCutted;	//切好的西红柿

/*============================CC ========*/
ID3D11ShaderResourceView* texturePlatebread;      //面包纹理
ID3D11ShaderResourceView* texturePlatemeat;
ID3D11ShaderResourceView* texturePlatevegetable;
ID3D11ShaderResourceView* texturePlatetomato;

//在人物头上显示的图片
ID3D11ShaderResourceView* textureHeadbread;      //面包纹理
ID3D11ShaderResourceView* textureHeadmeat;
ID3D11ShaderResourceView* textureHeadvegetable;
ID3D11ShaderResourceView* textureHeadtomato;

ID3D11ShaderResourceView* textureRubbishl;   //垃圾箱
ID3D11ShaderResourceView* textureRubbishr;
ID3D11ShaderResourceView* texturePot;         //锅
ID3D11ShaderResourceView* textureCuttingboard;  //菜板
ID3D11ShaderResourceView* textureOut;     //出口
//下面为三种煎肉状态的贴图
ID3D11ShaderResourceView* textureCooking;     //正在煎肉
ID3D11ShaderResourceView* textureCookedWell;     //肉煎好了
ID3D11ShaderResourceView* textureOverCooked;     //肉煎糊了
ID3D11ShaderResourceView* textureOverCookedMeat;     //糊了的肉
/*============================CC ========*/

//时间数字纹理
ID3D11ShaderResourceView* textureScore1;
ID3D11ShaderResourceView* textureScore2;
ID3D11ShaderResourceView* textureScore3;
ID3D11ShaderResourceView* textureScore4;
ID3D11ShaderResourceView* textureScore5;
ID3D11ShaderResourceView* textureScore6;
ID3D11ShaderResourceView* textureScore7;
ID3D11ShaderResourceView* textureScore8;
ID3D11ShaderResourceView* textureScore9;
ID3D11ShaderResourceView* textureScore0;

//游戏开始界面纹理
ID3D11ShaderResourceView* startGame;

ID3D11BlendState* blendStateAlpha;        //混合状态
ID3D11RasterizerState* noCullRS;          //背面消隐状态


//指示四种食物的获取状态,二维数组分别表示两个人
int IsTomato[2] = { 0,0 };
int IsBread[2] = { 0,0 };
int IsMeat[2] = { 0,0 };
int IsLettuce[2] = { 0,0 };
//指示四种准备好的食物的获取状态，二维数组分别表示两个人
int IsTomatoWell[2] = { 0,0 };
int IsBreadWell[2] = { 0,0 };
int IsMeatWell[2] = { 0,0 };
int IsLettuceWell[2] = { 0,0 };
//指示四种允许上传送带的食物状态
int IsTomatoInSC = 0;
int IsBreadInSC = 0;
int IsMeatWellInSC = 0;
int IsLettuceInSC = 0;
int Cooking = 0;//指示是否在煎肉
int Cutting = 0;//指示是否在切菜
int IsCuttedWell = 0;//指示是否切好了菜
int IsMeatCookedWell = 0;//指示肉是否煎好
int IsHaveOverCookedMeat = 0;//指示人物头上是否有煎糊了的肉
int FoodVarity[4] = { 0,0,0,0 };//指示输出窗口的已放入食物种类分别表示面包，生菜，番茄和肉
float PutToSCXMove = 0.0f;//指示食材应该在传送带上的起始位置
int Menu[4] = { 0,0,0,0 };  //菜单


ID3D11ShaderResourceView* texturePlayer1GetFood = texturePlateOfPalyer1;  // 玩家1取得食物的纹理
ID3D11ShaderResourceView* texturePlayer2GetFood;
static int getFoodPlayer1VertexNums = 0;      //玩家1取得食物的顶点数
static int getFoodPlayer2VertexNums = 0;

void SetLightEffect(Light light);
void putFoodInScroll(int playerNum, float startPos, float stepLen, float timeDelta, int vertexNums, ID3D11ShaderResourceView* textureTemp);
//改变人物头顶盘子的纹理
void setTextureOfPlayerHead(int vertexNums, float tmpMoveX, float tmpMoveZ, ID3D11ShaderResourceView* textureTempChange);

/*============================CC ========*/
bool CanGetFood(float x, float z);//判断可不可以拿到菜
bool CanCookMeat(float x, float z);//判断是不是可以煮菜
bool CanCutVegtable(float x, float z);//判断是不是可以切菜
bool CanPutInScroll(float x, float z);//判断是不是可以将食物放上传送带
bool CanGetFromScroll(float x, float z);//判断是不是可以从传送带上取食物
bool SendOutFood(float x, float z);//判断食物是否被放入输出窗口 分别是 面包 生菜 番茄 肉
bool CanThrowFood(float x, float z);//判断下面的人是不是可以向垃圾桶丢食物
/*============================CC ========*/
void createmenu(int i);      //生成菜单
static int RewardScore = 8;           //做好一道菜，玩家得到8分
GameTimer mytimer = GameTimer();     //游戏计时器
GameTimer cookingTimer = GameTimer();     //用于计时煎肉花了多久
static bool IsGetScore = false;     //判定这道菜能不能得分，false表示菜与菜单要求不符合；true表示符号要求，可以得分
static bool GameStart = false;          //游戏开始标志
static bool GameOver = false;          //游戏结束标志


//定义一个顶点结构，这个顶点包含坐标和法向量和纹理坐标
struct Vertex
{
	XMFLOAT3 Pos;   //顶点
	XMFLOAT3 Normal;	//光照法向量
	XMFLOAT2 Tex;	//纹理
};

//这里需要定义箱子，池子，以及水面的顶点
Vertex vertices[] =
{
	//下侧边界
	{ XMFLOAT3(-7.0f,0.3f,-4.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
	{ XMFLOAT3(8.0f, 0.3f, -4.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(15.0f, 0.0f) },
	{ XMFLOAT3(-7.0f, 0.3f, -5.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(0.0f, 1.0f) },

	{ XMFLOAT3(8.0f, 0.3f, -4.0f),XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(15.0f, 0.0f) },
	{ XMFLOAT3(8.0f,0.3f,-5.0f),XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(15.0f, 1.0f) },
	{ XMFLOAT3(-7.0f, 0.3f, -5.0f),XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(0.0f, 1.0f) },
	//右侧边界
	{ XMFLOAT3(8.0f,0.3f,0.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(1.0f, 0.0f) },
	{ XMFLOAT3(8.0f, 0.3f, -5.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(1.0f, 5.0f) },
	{ XMFLOAT3(7.0f, 0.3f, 0.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },

	{ XMFLOAT3(7.0f, 0.3f, 0.0f),XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
	{ XMFLOAT3(8.0f,0.3f,-5.0f),XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(1.0f, 5.0f) },
	{ XMFLOAT3(7.0f, 0.3f, -5.0f),XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(0.0f, 5.0f) },

	{ XMFLOAT3(8.0f,0.3f,6.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(1.0f, 0.0f) },
	{ XMFLOAT3(8.0f, 0.3f, 1.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(1.0f, 5.0f) },
	{ XMFLOAT3(7.0f, 0.3f, 6.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },

	{ XMFLOAT3(7.0f, 0.3f, 6.0f),XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
	{ XMFLOAT3(8.0f,0.3f,1.0f),XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(1.0f, 5.0f) },
	{ XMFLOAT3(7.0f, 0.3f, 1.0f),XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(0.0f, 5.0f) },
	//上侧边界
	{ XMFLOAT3(-7.0f,0.3f,5.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(0.0f, 1.0f) },
	{ XMFLOAT3(-7.0f, 0.3f, 6.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
	{ XMFLOAT3(8.0f, 0.3f, 5.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(15.0f, 1.0f) },

	{ XMFLOAT3(-7.0f,0.3f,6.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
	{ XMFLOAT3(8.0f, 0.3f, 6.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(15.0f, 0.0f) },
	{ XMFLOAT3(8.0f, 0.3f, 5.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(15.0f, 1.0f) },
	//左侧边界
	{ XMFLOAT3(-6.0f,0.3f,0.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(1.0f, 0.0f) },
	{ XMFLOAT3(-6.0f, 0.3f, -5.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(1.0f, 5.0f) },
	{ XMFLOAT3(-7.0f, 0.3f, 0.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },

	{ XMFLOAT3(-7.0f, 0.3f, 0.0f),XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
	{ XMFLOAT3(-6.0f,0.3f,-5.0f),XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(1.0f, 5.0f) },
	{ XMFLOAT3(-7.0f, 0.3f, -5.0f),XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(0.0f, 5.0f) },

	{ XMFLOAT3(-6.0f,0.3f,6.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(1.0f, 0.0f) },
	{ XMFLOAT3(-6.0f, 0.3f, 1.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(1.0f, 5.0f) },
	{ XMFLOAT3(-7.0f, 0.3f, 6.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },

	{ XMFLOAT3(-7.0f, 0.3f, 6.0f),XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
	{ XMFLOAT3(-6.0f,0.3f,1.0f),XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(1.0f, 5.0f) },
	{ XMFLOAT3(-7.0f, 0.3f, 1.0f),XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(0.0f,5.0f) },
	//传送带
	{ XMFLOAT3(-7.0f,0.3f,1.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
	{ XMFLOAT3(8.0f, 0.3f, 0.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(15.0f, 1.0f) },
	{ XMFLOAT3(-7.0f, 0.3f, 0.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(0.0f, 1.0f) },

	{ XMFLOAT3(-7.0f,0.3f,1.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
	{ XMFLOAT3(8.0f, 0.3f, 1.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(15.0f, 0.0f) },
	{ XMFLOAT3(8.0f, 0.3f, 0.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(15.0f, 1.0f) },

	//中轴线墙体从左到右
	{ XMFLOAT3(-6.0f,0.3f,2.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
	{ XMFLOAT3(-3.0f, 0.3f, 2.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(3.0f, 0.0f) },
	{ XMFLOAT3(-3.0f, 0.3f, 1.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(3.0f, 1.0f) },

	{ XMFLOAT3(-6.0f,0.3f,2.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
	{ XMFLOAT3(-3.0f, 0.3f, 1.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(3.0f, 1.0f) },
	{ XMFLOAT3(-6.0f, 0.3f, 1.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(0.0f, 1.0f) },

	{ XMFLOAT3(-3.0f,0.3f,0.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
	{ XMFLOAT3(0.0f, 0.3f, 0.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(3.0f, 0.0f) },
	{ XMFLOAT3(0.0f, 0.3f, -1.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(3.0f, 1.0f) },

	{ XMFLOAT3(-3.0f,0.3f,0.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
	{ XMFLOAT3(0.0f, 0.3f, -1.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(3.0f, 1.0f) },
	{ XMFLOAT3(-3.0f, 0.3f, -1.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(0.0f, 1.0f) },

	{ XMFLOAT3(1.0f,0.3f,2.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
	{ XMFLOAT3(4.0f, 0.3f, 2.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(3.0f, 0.0f) },
	{ XMFLOAT3(4.0f, 0.3f, 1.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(3.0f, 1.0f) },

	{ XMFLOAT3(1.0f,0.3f,2.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
	{ XMFLOAT3(4.0f, 0.3f, 1.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(3.0f, 1.0f) },
	{ XMFLOAT3(1.0f, 0.3f, 1.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(0.0f, 1.0f) },

	{ XMFLOAT3(4.0f,0.3f,0.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
	{ XMFLOAT3(7.0f, 0.3f, 0.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(3.0f, 0.0f) },
	{ XMFLOAT3(7.0f, 0.3f, -1.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(3.0f, 1.0f) },

	{ XMFLOAT3(4.0f,0.3f,0.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
	{ XMFLOAT3(7.0f, 0.3f, -1.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(3.0f, 1.0f) },
	{ XMFLOAT3(4.0f, 0.3f, -1.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(0.0f, 1.0f) },
	//22*3
	//右下方侧面贴图 42
	{ XMFLOAT3(7.0f,0.3f,-1.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(1.0f, 0.0f) },
	{ XMFLOAT3(7.0f,0.3f,-4.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(1.0f, 3.0f) },
	{ XMFLOAT3(7.0f,0.0f,-1.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },

	{ XMFLOAT3(7.0f,0.0f,-1.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
	{ XMFLOAT3(7.0f,0.3f,-4.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(1.0f, 3.0f) },
	{ XMFLOAT3(7.0f,0.0f,-4.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(0.0f, 3.0f) },

	//右侧中部上方侧面 48
	{ XMFLOAT3(4.0f,0.3f,-1.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(0.0f, 1.0f) },
	{ XMFLOAT3(7.0f,0.3f,-1.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(3.0f, 1.0f) },
	{ XMFLOAT3(7.0f,0.0f,-1.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(3.0f, 0.0f) },

	{ XMFLOAT3(4.0f,0.3f,-1.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(0.0f, 1.0f) },
	{ XMFLOAT3(7.0f,0.0f,-1.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(3.0f, 0.0f) },
	{ XMFLOAT3(4.0f,0.0f,-1.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },

	{ XMFLOAT3(4.0f,0.0f,0.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
	{ XMFLOAT3(4.0f,0.3f,0.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(1.0f, 0.0f) },
	{ XMFLOAT3(4.0f,0.3f,-1.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) },

	{ XMFLOAT3(4.0f,0.0f,0.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
	{ XMFLOAT3(4.0f,0.3f,-1.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) },
	{ XMFLOAT3(4.0f,0.0f,-1.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(0.0f, 1.0f) },

	//右侧中部上方
	{ XMFLOAT3(4.0f,0.3f,0.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(0.0f, 1.0f) },
	{ XMFLOAT3(7.0f,0.3f,0.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(3.0f, 1.0f) },
	{ XMFLOAT3(7.0f,0.3f,-1.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(3.0f, 0.0f) },

	{ XMFLOAT3(4.0f,0.3f,0.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(0.0f, 1.0f) },
	{ XMFLOAT3(7.0f,0.3f,-1.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(3.0f, 0.0f) },
	{ XMFLOAT3(4.0f,0.3f,-1.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },

	//左侧侧面
	{ XMFLOAT3(-6.0f,0.3f,0.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(1.0f, 0.0f) },
	{ XMFLOAT3(-6.0f,0.0f,0.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
	{ XMFLOAT3(-6.0f,0.0f,-4.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(0.0f, 3.0f) },

	{ XMFLOAT3(-6.0f,0.3f,0.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(1.0f, 0.0f) },
	{ XMFLOAT3(-6.0f,0.0f,-4.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(0.0f, 3.0f) },
	{ XMFLOAT3(-6.0f,0.3f,-4.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(1.0f, 3.0f) },

	//左上传送带侧面
	{ XMFLOAT3(-6.0f,0.3f,0.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
	{ XMFLOAT3(-3.0f,0.3f,0.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(3.0f, 0.0f) },
	{ XMFLOAT3(-3.0f,0.0f,0.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(3.0f, 1.0f) },

	{ XMFLOAT3(-6.0f,0.3f,0.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
	{ XMFLOAT3(-3.0f,0.0f,0.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(3.0f, 1.0f) },
	{ XMFLOAT3(-6.0f,0.0f,0.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(0.0f, 1.0f) },

	//右传送带侧面
	{ XMFLOAT3(0.0f,0.3f,0.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
	{ XMFLOAT3(4.0f,0.3f,0.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(4.0f, 0.0f) },
	{ XMFLOAT3(4.0f,0.0f,0.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(4.0f, 1.0f) },

	{ XMFLOAT3(0.0f,0.3f,0.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
	{ XMFLOAT3(4.0f,0.0f,0.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(4.0f, 1.0f) },
	{ XMFLOAT3(0.0f,0.0f,0.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(0.0f, 1.0f) },

	//左下侧中轴线墙壁侧面
	{ XMFLOAT3(-3.0f,0.3f,-1.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
	{ XMFLOAT3(0.0f,0.3f,-1.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(3.0f, 0.0f) },
	{ XMFLOAT3(0.0f,0.0f,-1.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(3.0f, 1.0f) },

	{ XMFLOAT3(-3.0f,0.3f,-1.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
	{ XMFLOAT3(0.0f,0.0f,-1.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(3.0f, 1.0f) },
	{ XMFLOAT3(-3.0f,0.0f,-1.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(0.0f, 1.0f) },
	//左上中轴线，侧面右
	{ XMFLOAT3(-3.0f,0.3f,2.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(1.0f, 0.0f) },
	{ XMFLOAT3(-3.0f,0.0f,2.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
	{ XMFLOAT3(-3.0f,0.0f,1.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(0.0f, 1.0f) },

	{ XMFLOAT3(-3.0f,0.3f,2.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(0.0f, 1.0f) },
	{ XMFLOAT3(-3.0f,0.0f,1.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(1.0f, 0.0f) },
	{ XMFLOAT3(-3.0f,0.3f,1.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) },
	//左下中轴线，侧面右
	{ XMFLOAT3(0.0f,0.3f,0.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(1.0f, 0.0f) },
	{ XMFLOAT3(0.0f,0.0f,0.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
	{ XMFLOAT3(0.0f,0.0f,-1.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(0.0f, 1.0f) },

	{ XMFLOAT3(0.0f,0.3f,0.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(0.0f, 1.0f) },
	{ XMFLOAT3(0.0f,0.0f,-1.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(1.0f, 0.0f) },
	{ XMFLOAT3(0.0f,0.3f,-1.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) },

	//上左墙壁侧面
	{ XMFLOAT3(-6.0f,0.3f,5.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(1.0f, 0.0f) },
	{ XMFLOAT3(-6.0f,0.0f,5.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
	{ XMFLOAT3(-6.0f,0.0f,2.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(0.0f, 3.0f) },

	{ XMFLOAT3(-6.0f,0.3f,5.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(1.0f, 0.0f) },
	{ XMFLOAT3(-6.0f, 0.0f, 2.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(0.0f, 3.0f) },
	{ XMFLOAT3(-6.0f,0.3f,2.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(1.0f, 3.0f) },

	//上边缘墙壁侧面
	{ XMFLOAT3(-6.0f,0.3f,5.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
	{ XMFLOAT3(7.0f,0.3f,5.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(13.0f, 0.0f) },
	{ XMFLOAT3(7.0f,0.0f,5.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(13.0f, 1.0f) },

	{ XMFLOAT3(-6.0f,0.3f,5.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
	{ XMFLOAT3(7.0f,0.0f,5.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(13.0f, 1.0f) },
	{ XMFLOAT3(-6.0f,0.0f,5.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(0.0f, 1.0f) },

	//右上输出窗口侧边
	{ XMFLOAT3(7.0f,0.3f,5.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(1.0f, 0.0f) },
	{ XMFLOAT3(7.0f,0.3f,3.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(1.0f, 2.0f) },
	{ XMFLOAT3(7.0f,0.0f,3.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(0.0f, 2.0f) },

	{ XMFLOAT3(7.0f,0.3f,5.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(1.0f, 0.0f) },
	{ XMFLOAT3(7.0f,0.0f,3.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(0.0f, 2.0f) },
	{ XMFLOAT3(7.0f,0.0f,5.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },


	//右上其他窗口边
	{ XMFLOAT3(7.0f,0.3f,3.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(1.0f, 0.0f) },
	{ XMFLOAT3(7.0f,0.3f,1.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(1.0f, 2.0f) },
	{ XMFLOAT3(7.0f,0.0f,1.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(0.0f, 2.0f) },

	{ XMFLOAT3(7.0f,0.3f,3.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(1.0f, 0.0f) },
	{ XMFLOAT3(7.0f,0.0f,1.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(0.0f, 2.0f) },
	{ XMFLOAT3(7.0f,0.0f,3.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
	//22*3 28*3
	//地板
	{ XMFLOAT3(10.0f,0.0f,10.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
	{ XMFLOAT3(10.0f,0.0f,-10.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(10.0f, 0.0f) },
	{ XMFLOAT3(-10.0f,0.0f,10.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(0.0f, 10.0f) },

	{ XMFLOAT3(-10.0f,0.0f,10.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(0.0f, 10.0f) },
	{ XMFLOAT3(10.0f,0.0f,-10.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(10.0f, 0.0f) },
	{ XMFLOAT3(-10.0f,0.0f,-10.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(10.0f,10.0f) },

	//接下来绘制周围四堵墙

	//左边墙 156
	{ XMFLOAT3(-7.0f,2.5f,6.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(4.0f, 0.0f) },
	{ XMFLOAT3(-7.0f,0.0f,6.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(4.0f, 2.5f) },
	{ XMFLOAT3(-7.0f,0.0f,-5.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(0.0f, 2.5f) },

	{ XMFLOAT3(-7.0f,2.5f,6.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(4.0f, 0.0f) },
	{ XMFLOAT3(-7.0f,0.0f,-5.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(0.0f, 2.5f) },
	{ XMFLOAT3(-7.0f,2.5f,-5.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },

	//下边墙 162
	{ XMFLOAT3(-7.0f,2.5f,-5.5f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
	{ XMFLOAT3(8.0f,2.5f,-5.5f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(11.0f, 0.0f) },
	{ XMFLOAT3(8.0f,0.0f,-5.5f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(11.0f, 2.5f) },

	{ XMFLOAT3(-7.0f,2.5f,-5.5f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
	{ XMFLOAT3(8.0f,0.0f,-5.5f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(11.0f, 2.5f) },
	{ XMFLOAT3(-7.0f,0.0f,-5.5f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(0.0f, 2.5f) },

	//上边墙 168
	{ XMFLOAT3(-7.0f,2.5f,6.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
	{ XMFLOAT3(8.0f,2.5f,6.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(6.0f, 0.0f) },
	{ XMFLOAT3(8.0f,0.0f,6.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(6.0f, 2.5f) },

	{ XMFLOAT3(-7.0f,2.5f,6.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
	{ XMFLOAT3(8.0f,0.0f,6.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(6.0f, 2.5f) },
	{ XMFLOAT3(-7.0f,0.0f,6.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(0.0f, 2.5f) },

	//右侧墙
	//上半段 174
	{ XMFLOAT3(8.0f,2.5f,6.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
	{ XMFLOAT3(8.0f,2.5f,5.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(0.5f, 0.0f) },
	{ XMFLOAT3(8.0f,0.0f,5.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(0.5f, 2.5f) },

	{ XMFLOAT3(8.0f,2.5f,6.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
	{ XMFLOAT3(8.0f,0.0f,5.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(0.5f, 2.5f) },
	{ XMFLOAT3(8.0f,0.0f,6.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(0.0f, 2.5f) },

	//下半段 180
	{ XMFLOAT3(8.0f,2.5f,3.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
	{ XMFLOAT3(8.0f,2.5f,-5.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(3.0f, 0.0f) },
	{ XMFLOAT3(8.0f,0.0f,-5.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(3.0f, 2.5f) },

	{ XMFLOAT3(8.0f,2.5f,3.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
	{ XMFLOAT3(8.0f,0.0f,-5.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(3.0f, 2.5f) },
	{ XMFLOAT3(8.0f,0.0f,3.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(0.0f, 2.5f) },

	//接下来绘制墙的顶部
			//左边墙顶部 186
	{ XMFLOAT3(-7.5f,2.5f,6.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(4.0f, 0.0f) },
	{ XMFLOAT3(-7.0f,2.5f,6.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(4.0f, 1.0f) },
	{ XMFLOAT3(-7.0f,2.5f,-5.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(0.0f, 1.0f) },

	{ XMFLOAT3(-7.5f,2.5f,6.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(4.0f, 0.0f) },
	{ XMFLOAT3(-7.0f,2.5f,-5.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(0.0f, 1.0f) },
	{ XMFLOAT3(-7.5f,2.5f,-5.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },


	//下边墙顶部 192
	{ XMFLOAT3(-7.5f,2.5f,-5.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
	{ XMFLOAT3(8.0f,2.5f,-5.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(6.0f, 0.0f) },
	{ XMFLOAT3(8.0f,2.5f,-5.5f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(6.0f, 1.0f) },

	{ XMFLOAT3(-7.5f,2.5f,-5.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
	{ XMFLOAT3(8.0f,2.5f,-5.5f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(6.0f, 1.0f) },
	{ XMFLOAT3(-7.5f,2.5f,-5.5f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(0.0f, 1.0f) },

	//上边墙顶部 198
	{ XMFLOAT3(-7.5f,2.5f,6.5), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
	{ XMFLOAT3(8.5f,2.5f,6.5), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(4.0f, 0.0f) },
	{ XMFLOAT3(8.5f,2.5,6.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(4.0f, 1.0f) },

	{ XMFLOAT3(-7.5f,2.5f,6.5), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
	{ XMFLOAT3(8.5f,2.5,6.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(4.0f, 1.0f) },
	{ XMFLOAT3(-7.5f,2.5,6.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(0.0f, 1.0f) },

	//右边墙顶部 204
	{ XMFLOAT3(8.0f,2.5f,6.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(4.0f, 0.0f) },
	{ XMFLOAT3(8.5f,2.5f,6.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(4.0f, 1.0f) },
	{ XMFLOAT3(8.5f,2.5,-5.5f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(0.0f, 1.0f) },

	{ XMFLOAT3(8.0f,2.5f,6.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(4.0f, 0.0f) },
	{ XMFLOAT3(8.5f,2.5f,-5.5f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(0.0f, 1.0f) },
	{ XMFLOAT3(8.0f,2.5f,-5.5f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },

	//右边墙上半部分，侧面 210
	{ XMFLOAT3(8.0f,2.5f,5.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
	{ XMFLOAT3(8.5f,2.5f,5.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
	{ XMFLOAT3(8.5f,0.0,5.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },

	{ XMFLOAT3(8.0f,2.5f,5.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
	{ XMFLOAT3(8.5f,0.0f,5.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
	{ XMFLOAT3(8.0f,0.0f,5.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },


	//人物的顶点  216
	{ XMFLOAT3(2.0f, 1.0f, -0.7f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(0.0f, 0.25f) },
	{ XMFLOAT3(1.0f, 1.0f, -0.7f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(0.167f, 0.25f) },
	{ XMFLOAT3(2.0f, 0.0f, -1.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(0.0f, 0.50f) },

	{ XMFLOAT3(2.0f, 0.0f, -1.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(0.0f, 0.50f) },
	{ XMFLOAT3(1.0f, 1.0f, -0.7f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(0.167f, 0.25f) },
	{ XMFLOAT3(1.0f, 0.0f, -1.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(0.167f, 0.50f) },

	//人物2的顶点  222
	{ XMFLOAT3(-2.0f, 1.0f, 2.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(0.0f, 0.25f) },
	{ XMFLOAT3(-1.0f, 1.0f, 2.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(0.167f, 0.25f) },
	{ XMFLOAT3(-2.0f, 0.0f, 2.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(0.0f, 0.50f) },

	{ XMFLOAT3(-2.0f, 0.0f, 2.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(0.0f, 0.50f) },
	{ XMFLOAT3(-1.0f, 1.0f, 2.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(0.167f, 0.25f) },
	{ XMFLOAT3(-1.0f, 0.0f, 2.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(0.167f, 0.50f) },

	//传送带  228
	{ XMFLOAT3(-7.0f, 0.3f, 1.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
	{ XMFLOAT3(8.0f, 0.3f, 0.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(15.33f, 1.0f) },
	{ XMFLOAT3(-7.0f, 0.3f, 0.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(0.0f, 1.0f) },

	{ XMFLOAT3(-7.0f,0.3f, 1.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
	{ XMFLOAT3(8.0f, 0.3f, 1.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(15.33f, 0.0f) },
	{ XMFLOAT3(8.0f, 0.3f, 0.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(15.33f, 1.0f) },

	{ XMFLOAT3(-7.0f,0.3f, 1.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
	{ XMFLOAT3(8.0f, 0.3f, 0.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(15.66f, 1.0f) },
	{ XMFLOAT3(-7.0f, 0.3f, 0.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(0.0f, 1.0f) },

	{ XMFLOAT3(-7.0f,0.3f, 1.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
	{ XMFLOAT3(8.0f, 0.3f, 1.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(15.66f, 0.0f) },
	{ XMFLOAT3(8.0f, 0.3f, 0.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(15.66f, 1.0f) },

	/*======================================cc =========*/
//盘子  240
{ XMFLOAT3(2.0f,0.35f,-4.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
{ XMFLOAT3(3.0f, 0.35f, -4.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(1.0f, 0.0f) },
{ XMFLOAT3(2.0f, 0.35f, -5.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(0.0f, 1.0f) },

{ XMFLOAT3(3.0f, 0.35f, -4.0f),XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(1.0f, 0.0f) },
{ XMFLOAT3(3.0f,0.35f,-5.0f),XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) },
{ XMFLOAT3(2.0f, 0.35f, -5.0f),XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(0.0f, 1.0f) },

{ XMFLOAT3(3.0f,0.35f,-4.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
{ XMFLOAT3(4.0f, 0.35f, -4.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(1.0f, 0.0f) },
{ XMFLOAT3(3.0f, 0.35f, -5.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(0.0f, 1.0f) },

{ XMFLOAT3(4.0f, 0.35f, -4.0f),XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(1.0f, 0.0f) },
{ XMFLOAT3(4.0f,0.35f,-5.0f),XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) },
{ XMFLOAT3(3.0f, 0.35f, -5.0f),XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(0.0f, 1.0f) },

{ XMFLOAT3(4.0f,0.35f,-4.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
{ XMFLOAT3(5.0f, 0.35f, -4.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(1.0f, 0.0f) },
{ XMFLOAT3(4.0f, 0.35f, -5.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(0.0f, 1.0f) },

{ XMFLOAT3(5.0f, 0.35f, -4.0f),XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(1.0f, 0.0f) },
{ XMFLOAT3(5.0f,0.35f,-5.0f),XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) },
{ XMFLOAT3(4.0f, 0.35f, -5.0f),XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(0.0f, 1.0f) },

{ XMFLOAT3(5.0f,0.35f,-4.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
{ XMFLOAT3(6.0f, 0.35f, -4.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(1.0f, 0.0f) },
{ XMFLOAT3(5.0f, 0.35f, -5.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(0.0f, 1.0f) },

{ XMFLOAT3(6.0f, 0.35f, -4.0f),XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(1.0f, 0.0f) },
{ XMFLOAT3(6.0f,0.35f,-5.0f),XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) },
{ XMFLOAT3(5.0f, 0.35f, -5.0f),XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(0.0f, 1.0f) },
//垃圾箱  264
{ XMFLOAT3(-7.0f,0.31f,1.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
{ XMFLOAT3(-6.0f, 0.31f, 0.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) },
{ XMFLOAT3(-7.0f, 0.31f, 0.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(0.0f, 1.0f) },

{ XMFLOAT3(-7.0f,0.31f,1.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
{ XMFLOAT3(-6.0f, 0.31f, 1.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(1.0f, 0.0f) },
{ XMFLOAT3(-6.0f, 0.31f, 0.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) },

{ XMFLOAT3(7.0f,0.31f,1.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
{ XMFLOAT3(8.0f, 0.31f, 0.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) },
{ XMFLOAT3(7.0f, 0.31f, 0.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(0.0f, 1.0f) },

{ XMFLOAT3(7.0f,0.31f,1.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
{ XMFLOAT3(8.0f, 0.31f, 1.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(1.0f, 0.0f) },
{ XMFLOAT3(8.0f, 0.31f, 0.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) },

//锅 276
{ XMFLOAT3(-6.0f,0.32f,-1.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(1.0f, 0.0f) },
{ XMFLOAT3(-6.0f, 0.32f, -2.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) },
{ XMFLOAT3(-7.0f, 0.32f, -1.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },

{ XMFLOAT3(-7.0f, 0.32f, -1.0f),XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
{ XMFLOAT3(-6.0f,0.32f,-2.0f),XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) },
{ XMFLOAT3(-7.0f, 0.32f, -2.0f),XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(0.0f, 1.0f) },

//菜板 282
{ XMFLOAT3(-1.0f,0.32f,5.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(0.0f, 1.0f) },
{ XMFLOAT3(-1.0f, 0.32f, 6.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
{ XMFLOAT3(0.0f, 0.32f, 5.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) },

{ XMFLOAT3(-1.0f,0.32f,6.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
{ XMFLOAT3(0.0f, 0.32f, 6.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(1.0f, 0.0f) },
{ XMFLOAT3(0.0f, 0.32f, 5.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) },

//出口 288
{ XMFLOAT3(8.5f,0.31f,5.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(1.0f, 0.0f) },
{ XMFLOAT3(8.5f, 0.31f, 3.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) },
{ XMFLOAT3(7.0f, 0.31f, 5.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },

{ XMFLOAT3(7.0f, 0.31f, 5.0f),XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
{ XMFLOAT3(8.5f,0.31f,3.0f),XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) },
{ XMFLOAT3(7.0f, 0.31f, 3.0f),XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(0.0f, 1.0f) },
/*======================================cc =========*/


// 食物test  294
{ XMFLOAT3(5.0f, 0.35f, 1.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
{ XMFLOAT3(6.0f, 0.35f, 0.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) },
{ XMFLOAT3(5.0f, 0.35f, 0.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(0.0f, 1.0f) },

{ XMFLOAT3(5.0f, 0.35f, 1.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
{ XMFLOAT3(6.0f, 0.35f, 1.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(1.0f, 0.0f) },
{ XMFLOAT3(6.0f, 0.35f, 0.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) },

//人物头顶的盘子 300
{ XMFLOAT3(1.7f, 1.4f, -0.2f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
{ XMFLOAT3(1.2f, 1.4f, -0.2f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(1.0f, 0.0f) },
{ XMFLOAT3(1.7f, 1.1f, -1.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(0.0f, 1.0f) },

{ XMFLOAT3(1.7f, 1.1f, -1.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(0.0f, 1.0f) },
{ XMFLOAT3(1.2f, 1.4f, -0.2f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(1.0f, 0.0f) },
{ XMFLOAT3(1.2f, 1.1f, -1.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) },

//人物2头顶的盘子 306
{ XMFLOAT3(-1.7f, 1.4f, 2.5f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
{ XMFLOAT3(-1.2f, 1.4f, 2.5f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(1.0f, 0.0f) },
{ XMFLOAT3(-1.7f, 1.1f, 2.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(0.0f, 1.0f) },

{ XMFLOAT3(-1.7f, 1.1f, 2.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(0.0f, 1.0f) },
{ XMFLOAT3(-1.2f, 1.4f, 2.5f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(1.0f, 0.0f) },
{ XMFLOAT3(-1.2f, 1.1f, 2.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) },


//垃圾箱  312
{ XMFLOAT3(-6.0f,0.31f,-4.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
{ XMFLOAT3(-5.0f, 0.31f, -5.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) },
{ XMFLOAT3(-6.0f, 0.31f, -5.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(0.0f, 1.0f) },

{ XMFLOAT3(-6.0f,0.31f,-4.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
{ XMFLOAT3(-5.0f, 0.31f, -4.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(1.0f, 0.0f) },
{ XMFLOAT3(-5.0f, 0.31f, -5.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) },

//菜单 318
{ XMFLOAT3(-4.5f, 5.5f, 0.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },  //左上
{ XMFLOAT3(-3.5f, 5.5f, 0.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(1.0f, 0.0f) },   //右上
{ XMFLOAT3(-3.5f, 4.5f, -1.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) },   //右下

{ XMFLOAT3(-4.5f, 5.5f, 0.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },//左上
{ XMFLOAT3(-3.5f, 4.5f, -1.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) }, //右下
{ XMFLOAT3(-4.5f, 4.5f, -1.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(0.0f, 1.0f) }, //左下

{ XMFLOAT3(-4.5f, 4.5f, -1.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },  //左上
{ XMFLOAT3(-4.17f, 4.5f, -1.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(1.0f, 0.0f) },   //右上
{ XMFLOAT3(-4.17f, 4.17f, -1.3f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) },   //右下

{ XMFLOAT3(-4.5f, 4.5f, -1.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },//左上
{ XMFLOAT3(-4.17f, 4.17f, -1.3f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) }, //右下
{ XMFLOAT3(-4.5f, 4.17f, -1.33f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(0.0f, 1.0f) }, //左下

{ XMFLOAT3(-4.16f, 4.5f, -1.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },  //左上
{ XMFLOAT3(-3.83f, 4.5f, -1.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(1.0f, 0.0f) },   //右上
{ XMFLOAT3(-3.83f, 4.17f,-1.33f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) },   //右下

{ XMFLOAT3(-4.16f, 4.5f, -1.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },//左上
{ XMFLOAT3(-3.83f, 4.17f,-1.33f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) }, //右下
{ XMFLOAT3(-4.16f, 4.17f, -1.33f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(0.0f, 1.0f) }, //左下

{ XMFLOAT3(-3.82f, 4.5f, -1.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },  //左上
{ XMFLOAT3(-3.5f, 4.5f, -1.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(1.0f, 0.0f) },   //右上
{ XMFLOAT3(-3.5f, 4.17f, -1.33f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) },   //右下

{ XMFLOAT3(-3.82f, 4.5f, -1.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },//左上
{ XMFLOAT3(-3.5f, 4.17f, -1.33f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) }, //右下
{ XMFLOAT3(-3.82f, 4.17f, -1.33f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(0.0f, 1.0f) }, //左下

	//时间面板  342
{ XMFLOAT3(-0.55f, 5.5f, 0.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },//左上
{ XMFLOAT3(-0.1f, 5.0f, 0.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) }, //右下
{ XMFLOAT3(-0.35f, 5.0f, 0.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(0.0f, 1.0f) }, //左下

{ XMFLOAT3(-0.55f, 5.5f, 0.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },  //左上
{ XMFLOAT3(-0.3f, 5.5f, 0.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(1.0f, 0.0f) },   //右上
{ XMFLOAT3(-0.1f, 5.0f, 0.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) },   //右下

//分数面板  348
{ XMFLOAT3(-0.55f, 5.5f, 0.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },//左上
{ XMFLOAT3(-0.3f, 5.0f, 0.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) }, //右下
{ XMFLOAT3(-0.55f, 5.0f, 0.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(0.0f, 1.0f) }, //左下

{ XMFLOAT3(-0.55f, 5.5f, 0.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },  //左上
{ XMFLOAT3(-0.3f, 5.5f, 0.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(1.0f, 0.0f) },   //右上
{ XMFLOAT3(-0.3f, 5.0f, 0.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) },   //右下

	//提醒玩家开始游戏面板  354
{ XMFLOAT3(-2.5f, 5.5f, -0.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
{ XMFLOAT3(1.0f, 1.0f, -0.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) },
{ XMFLOAT3(-2.5f, 1.0f, -0.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(0.0f, 1.0f) },

{ XMFLOAT3(-2.5f, 5.5f, -0.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
{ XMFLOAT3(1.0f, 5.5f, -0.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(1.0f, 0.0f) },
{ XMFLOAT3(1.0f, 1.0f, -0.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) },

};

//**************以下为框架函数******************
bool Setup()
{
	//这里主要包含5个主要步骤
	//第一步载入外部文件（包括fx文件及图像文件）
	//第二步创建各种渲染状态
	//第三步创建输入布局
	//第四步创建顶点缓存
	//第五步设置材质和光照
	//*************第一步载入外部文件（包括fx文件及图像文件）****************************
	hr = S_OK;              //声明HRESULT的对象用于记录函数调用是否成功
	ID3DBlob* pTechBlob = NULL;     //声明ID3DBlob的对象用于存放从文件读取的信息
	//从我们之前建立的.fx文件读取着色器相关信息
	hr = D3DX11CompileFromFile(L"Shader.fx", NULL, NULL, NULL, "fx_5_0",
		D3DCOMPILE_ENABLE_STRICTNESS, 0, NULL, &pTechBlob, NULL, NULL);
	if (FAILED(hr))
	{
		::MessageBox(NULL, L"fx文件载入失败", L"Error", MB_OK); //如果读取失败，弹出错误信息
		return hr;
	}
	//调用D3DX11CreateEffectFromMemory创建ID3DEffect对象
	hr = D3DX11CreateEffectFromMemory(pTechBlob->GetBufferPointer(),
		pTechBlob->GetBufferSize(), 0, device, &effect);

	if (FAILED(hr))
	{
		::MessageBox(NULL, L"创建Effect失败", L"Error", MB_OK);  //创建失败，弹出错误信息
		return hr;
	}
	//从外部图像文件载入纹理
	{
		////箱子纹理
		D3DX11CreateShaderResourceViewFromFile(device, L"img/desktop.jpg", NULL, NULL, &textureBox, NULL);
		////地板的纹理
		D3DX11CreateShaderResourceViewFromFile(device, L"img/floor.jpg", NULL, NULL, &textureFloor, NULL);
		////抽屉纹理
		D3DX11CreateShaderResourceViewFromFile(device, L"img/drawer.jpg", NULL, NULL, &textureDrawer, NULL);
		//墙纹理
		D3DX11CreateShaderResourceViewFromFile(device, L"img/Wall.PNG", NULL, NULL, &textureWall, NULL);
		//墙顶部
		D3DX11CreateShaderResourceViewFromFile(device, L"img/WallTop.PNG", NULL, NULL, &textureWallTop, NULL);
		//传送带纹理
		D3DX11CreateShaderResourceViewFromFile(device, L"img/ScrollLine.PNG", NULL, NULL, &textureScrollLine, NULL);
		//人物纹理
		D3DX11CreateShaderResourceViewFromFile(device, L"img/Role1.png", NULL, NULL, &texturePlayer1, NULL);
		//人物2纹理
		D3DX11CreateShaderResourceViewFromFile(device, L"img/Role2.png", NULL, NULL, &texturePlayer2, NULL);
		//test食物纹理
		D3DX11CreateShaderResourceViewFromFile(device, L"food/plate-vegetable.png", NULL, NULL, &textureVegetable, NULL);
		//人物1头顶盘子纹理
		D3DX11CreateShaderResourceViewFromFile(device, L"food/plate.png", NULL, NULL, &texturePlateOfPalyer1, NULL);

		//面包、肉、番茄、蔬菜、纹理
		D3DX11CreateShaderResourceViewFromFile(device, L"food/plate-bread.png", NULL, NULL, &texturePlatebread, NULL);
		D3DX11CreateShaderResourceViewFromFile(device, L"food/plate-meat.png", NULL, NULL, &texturePlatemeat, NULL);
		D3DX11CreateShaderResourceViewFromFile(device, L"food/plate-tomato.png", NULL, NULL, &texturePlatetomato, NULL);
		D3DX11CreateShaderResourceViewFromFile(device, L"food/plate-vegetable.png", NULL, NULL, &texturePlatevegetable, NULL);

		//人物头顶上的面包、肉、番茄、蔬菜、纹理
		D3DX11CreateShaderResourceViewFromFile(device, L"food/head-bread.png", NULL, NULL, &textureHeadbread, NULL);
		D3DX11CreateShaderResourceViewFromFile(device, L"food/head-meat.png", NULL, NULL, &textureHeadmeat, NULL);
		D3DX11CreateShaderResourceViewFromFile(device, L"food/head-tomato.png", NULL, NULL, &textureHeadtomato, NULL);
		D3DX11CreateShaderResourceViewFromFile(device, L"food/head-vegetable.png", NULL, NULL, &textureHeadvegetable, NULL);
		//人物头顶上的煎糊了的肉
		D3DX11CreateShaderResourceViewFromFile(device, L"food/head-meat-overcooked.png", NULL, NULL, &textureOverCookedMeat, NULL);

		//垃圾箱
		D3DX11CreateShaderResourceViewFromFile(device, L"food/Rubbishl.png", NULL, NULL, &textureRubbishl, NULL);
		D3DX11CreateShaderResourceViewFromFile(device, L"food/Rubbishr.png", NULL, NULL, &textureRubbishr, NULL);
		//锅
		D3DX11CreateShaderResourceViewFromFile(device, L"food/pot.png", NULL, NULL, &texturePot, NULL);
		//菜板
		D3DX11CreateShaderResourceViewFromFile(device, L"food/Cuttingboard.png", NULL, NULL, &textureCuttingboard, NULL);
		//出口
		D3DX11CreateShaderResourceViewFromFile(device, L"img/out.PNG", NULL, NULL, &textureOut, NULL);
		//切好的食物纹理
		D3DX11CreateShaderResourceViewFromFile(device, L"food/bread-cutted.png", NULL, NULL, &textureBreadCutted, NULL);
		D3DX11CreateShaderResourceViewFromFile(device, L"food/meat-cooked.png", NULL, NULL, &textureMeatCooked, NULL);
		D3DX11CreateShaderResourceViewFromFile(device, L"food/lettuce-cutted.png", NULL, NULL, &textureLettuceCutted, NULL);
		D3DX11CreateShaderResourceViewFromFile(device, L"food/tomato-cutted.png", NULL, NULL, &textureTomatoCutted, NULL);

		//煎肉时期需要用到的纹理
		D3DX11CreateShaderResourceViewFromFile(device, L"food/cooking.png", NULL, NULL, &textureCooking, NULL);
		D3DX11CreateShaderResourceViewFromFile(device, L"food/cooked.png", NULL, NULL, &textureCookedWell, NULL);
		D3DX11CreateShaderResourceViewFromFile(device, L"food/overcooked.png", NULL, NULL, &textureOverCooked, NULL);

		//数字纹理
		D3DX11CreateShaderResourceViewFromFile(device, L"number/1.png", NULL, NULL, &textureScore1, NULL);
		D3DX11CreateShaderResourceViewFromFile(device, L"number/2.png", NULL, NULL, &textureScore2, NULL);
		D3DX11CreateShaderResourceViewFromFile(device, L"number/3.png", NULL, NULL, &textureScore3, NULL);
		D3DX11CreateShaderResourceViewFromFile(device, L"number/4.png", NULL, NULL, &textureScore4, NULL);
		D3DX11CreateShaderResourceViewFromFile(device, L"number/5.png", NULL, NULL, &textureScore5, NULL);
		D3DX11CreateShaderResourceViewFromFile(device, L"number/6.png", NULL, NULL, &textureScore6, NULL);
		D3DX11CreateShaderResourceViewFromFile(device, L"number/7.png", NULL, NULL, &textureScore7, NULL);
		D3DX11CreateShaderResourceViewFromFile(device, L"number/8.png", NULL, NULL, &textureScore8, NULL);
		D3DX11CreateShaderResourceViewFromFile(device, L"number/9.png", NULL, NULL, &textureScore9, NULL);
		D3DX11CreateShaderResourceViewFromFile(device, L"number/0.png", NULL, NULL, &textureScore0, NULL);
		//提醒玩家游戏状态的纹理
		D3DX11CreateShaderResourceViewFromFile(device, L"img/StartGame.png", NULL, NULL, &startGame, NULL);
	}
	//纹理初始化结束


	//*************第一步载入外部文件（包括fx文件及图像文件）****************************

	//*************第二步创建各种渲染状态************************************************
	//先创建一个混合状态的描述
	D3D11_BLEND_DESC blendDesc;
	ZeroMemory(&blendDesc, sizeof(blendDesc)); //清零操作
	blendDesc.AlphaToCoverageEnable = false;     //关闭AlphaToCoverage多重采样技术
	blendDesc.IndependentBlendEnable = false;    //不针对多个RenderTarget使用不同的混合状态
	//只针对RenderTarget[0]设置绘制混合状态，忽略1-7
	blendDesc.RenderTarget[0].BlendEnable = true;                   //开启混合
	blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;     //设置源因子
	blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;//设置目标因子
	blendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;         //混合操作
	blendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;      //源混合百分比因子
	blendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;    //目标混合百分比因子
	blendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;    //混合百分比的操作
	blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;  //写掩码
	//创建ID3D11BlendState接口
	device->CreateBlendState(&blendDesc, &blendStateAlpha);

	//关闭背面消隐
	D3D11_RASTERIZER_DESC ncDesc;
	ZeroMemory(&ncDesc, sizeof(ncDesc));  //清零操作
	ncDesc.CullMode = D3D11_CULL_NONE;   //剔除特定朝向的三角形，这里不剔除，即全部绘制
	ncDesc.FillMode = D3D11_FILL_SOLID;  //填充模式，这里为利用三角形填充
	ncDesc.FrontCounterClockwise = false;//是否设置逆时针绕续的三角形为正面
	ncDesc.DepthClipEnable = true;       //开启深度裁剪
	//创建一个关闭背面消隐的状态，在需要用的时候才设置给设备上下文
	if (FAILED(device->CreateRasterizerState(&ncDesc, &noCullRS)))
	{
		MessageBox(NULL, L"Create 'NoCull' rasterizer state failed!", L"Error", MB_OK);
		return false;
	}
	//*************第二步创建各种渲染状态************************************************

	//*************第三步创建输入布局****************************************************
	//用GetTechniqueByName获取ID3DX11EffectTechnique的对象
	//先设置默认的technique到Effect
	technique = effect->GetTechniqueByName("TexTech");                //默认Technique

	//D3DX11_PASS_DESC结构用于描述一个Effect Pass
	D3DX11_PASS_DESC PassDesc;
	//利用GetPassByIndex获取Effect Pass
	//再利用GetDesc获取Effect Pass的描述，并存如PassDesc对象中
	technique->GetPassByIndex(0)->GetDesc(&PassDesc);

	//创建并设置输入布局
	//这里我们定义一个D3D11_INPUT_ELEMENT_DESC数组，
	//由于我们定义的顶点结构包括位置坐标和法向量，所以这个数组里有两个元素
	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0,  D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"NORMAL",   0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0}
	};
	//layout元素个数
	UINT numElements = ARRAYSIZE(layout);
	//调用CreateInputLayout创建输入布局
	hr = device->CreateInputLayout(layout, numElements, PassDesc.pIAInputSignature,
		PassDesc.IAInputSignatureSize, &vertexLayout);
	//设置生成的输入布局到设备上下文中
	immediateContext->IASetInputLayout(vertexLayout);
	if (FAILED(hr))
	{
		::MessageBox(NULL, L"创建Input Layout失败", L"Error", MB_OK);
		return hr;
	}
	//*************第三步创建输入布局****************************************************

	//*************第四步创建顶点缓存****************************************************

	UINT vertexCount = ARRAYSIZE(vertices);
	//创建顶点缓存，方法同实验4一样
	//首先声明一个D3D11_BUFFER_DESC的对象bd
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(Vertex) * vertexCount;      //注意：由于这里定义了24个顶点所以要乘以24
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;  //注意：这里表示创建的是顶点缓存
	bd.CPUAccessFlags = 0;

	//声明一个D3D11_SUBRESOURCE_DATA数据用于初始化子资源
	D3D11_SUBRESOURCE_DATA InitData;
	ZeroMemory(&InitData, sizeof(InitData));
	InitData.pSysMem = vertices;         //设置需要初始化的数据，这里的数据就是顶点数组
	InitData.SysMemPitch = 0;
	InitData.SysMemSlicePitch = 0;

	//声明一个ID3D11Buffer对象作为顶点缓存
	ID3D11Buffer* vertexBuffer;
	//调用CreateBuffer创建顶点缓存
	hr = device->CreateBuffer(&bd, &InitData, &vertexBuffer);
	if (FAILED(hr))
	{
		::MessageBox(NULL, L"创建VertexBuffer失败", L"Error", MB_OK);
		return hr;
	}

	UINT stride = sizeof(Vertex);                 //获取Vertex的大小作为跨度
	UINT offset = 0;                              //设置偏移量为0
	//设置顶点缓存，参数的解释见实验4
	immediateContext->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);
	//指定图元类型，D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST表示图元为三角形
	immediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	//*************第四步创建顶点缓存****************************************************

	//*************第五步设置材质和光照**************************************************
	//池子地板及墙的材质
	floorMaterial.ambient = XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f);
	floorMaterial.diffuse = XMFLOAT4(1.f, 1.f, 1.f, 1.0f);
	floorMaterial.specular = XMFLOAT4(0.3f, 0.3f, 0.3f, 16.0f);
	floorMaterial.power = 5.0f;
	//箱子材质
	boxMaterial.ambient = XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f);
	boxMaterial.diffuse = XMFLOAT4(1.f, 1.f, 1.f, 1.0f);
	boxMaterial.specular = XMFLOAT4(0.3f, 0.3f, 0.3f, 16.0f);
	boxMaterial.power = 5.0f;
	//水面材质
	waterMaterial.ambient = XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f);
	waterMaterial.diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);   //水面的alpha值为0.6，即其透明度为40%
	waterMaterial.specular = XMFLOAT4(0.3f, 0.3f, 0.3f, 16.0f);
	waterMaterial.power = 5.0f;

	// 设置光源
	Light dirLight, pointLight, spotLight;
	// 方向光只需要设置：方向、3种光照强度
	dirLight.type = 0;
	dirLight.direction = XMFLOAT4(0.0f, -1.0f, 0.0f, 1.0f);
	dirLight.ambient = XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f);   //前三位分别表示红绿蓝光的强度
	dirLight.diffuse = XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f);   //同上
	dirLight.specular = XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f);  //同上

	// 点光源需要设置：位置、3中光照强度、3个衰减因子
	pointLight.type = 1;
	pointLight.position = XMFLOAT4(0.0f, 5.0f, 0.0f, 1.0f); //光源位置
	pointLight.ambient = XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f);   //前三位分别表示红绿蓝光的强度
	pointLight.diffuse = XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f);   //同上
	pointLight.specular = XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f);  //同上
	pointLight.attenuation0 = 0;      //常量衰减因子
	pointLight.attenuation1 = 0.1f;   //一次衰减因子
	pointLight.attenuation2 = 0;      //二次衰减因子

	// 聚光灯需要设置Light结构中所有的成员
	spotLight.type = 2;
	spotLight.position = XMFLOAT4(0.0f, 10.0f, 0.0f, 1.0f); //光源位置
	spotLight.direction = XMFLOAT4(0.0f, -1.0f, 0.0f, 1.0f); //光照方向
	spotLight.ambient = XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f);   //前三位分别表示红绿蓝光的强度
	spotLight.diffuse = XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f);   //同上
	spotLight.specular = XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f);  //同上
	spotLight.attenuation0 = 0;    //常量衰减因子
	spotLight.attenuation1 = 0.1f; //一次衰减因子
	spotLight.attenuation2 = 0;    //二次衰减因子
	spotLight.alpha = XM_PI / 6;   //内锥角度
	spotLight.beta = XM_PI / 3;    //外锥角度
	spotLight.fallOff = 1.0f;      //衰减系数，一般为1.0

	light[0] = dirLight;
	light[1] = pointLight;
	light[2] = spotLight;
	//*************第五步设置材质和光照**************************************************

	return true;
}

void Cleanup()
{
	//释放全局指针
	if (renderTargetView) renderTargetView->Release();
	if (immediateContext) immediateContext->Release();
	if (swapChain) swapChain->Release();
	if (device) device->Release();

	if (vertexLayout) vertexLayout->Release();
	if (effect) effect->Release();
	if (depthStencilView) depthStencilView->Release();
	if (textureBox) textureBox->Release();
	if (textureFloor) textureFloor->Release();
	if (texturePlayer1) texturePlayer1->Release();
	if (texturePlayer2) texturePlayer2->Release();
	if (textureVegetable) textureVegetable->Release();
	if (texturePlateOfPalyer1) texturePlateOfPalyer1->Release();
	if (textureBreadCutted) textureBreadCutted->Release();

	if (textureCookedWell) textureCookedWell->Release();
	if (textureCooking) textureCooking->Release();
	if (textureCuttingboard) textureCuttingboard->Release();
	if (textureDrawer) textureDrawer->Release();
	if (textureLettuceCutted) textureLettuceCutted->Release();
	if (textureMeatCooked) textureMeatCooked->Release();
	if (textureOut) textureOut->Release();

	if (textureOverCooked) textureOverCooked->Release();
	if (texturePlatebread) texturePlatebread->Release();
	if (texturePlatemeat) texturePlatemeat->Release();
	if (texturePlatetomato) texturePlatetomato->Release();
	if (texturePlayer1GetFood) texturePlayer1GetFood->Release();
	if (texturePlayer2GetFood) texturePlayer2GetFood->Release();
	if (texturePot) texturePot->Release();
	if (textureRubbishl) textureRubbishl->Release();
	if (textureRubbishr) textureRubbishr->Release();

	if (textureScore0) textureScore0->Release();
	if (textureScore1) textureScore1->Release();
	if (textureScore2) textureScore2->Release();
	if (textureScore3) textureScore3->Release();
	if (textureScore4) textureScore4->Release();
	if (textureScore5) textureScore5->Release();
	if (textureScore6) textureScore6->Release();
	if (textureScore7) textureScore7->Release();
	if (textureScore8) textureScore8->Release();
	if (textureScore9) textureScore9->Release();
	if (textureScrollLine) textureScrollLine->Release();
	if (textureTomatoCutted) textureTomatoCutted->Release();
	if (textureWall) textureWall->Release();
	if (textureWallTop) textureWallTop->Release();


	if (blendStateAlpha) blendStateAlpha->Release();
	if (noCullRS) noCullRS->Release();

}

bool Display(float timeDelta)
{
	if (device)
	{
		ID3D11ShaderResourceView* textureNumber[] = { textureScore0, textureScore1, textureScore2, textureScore3,
			textureScore4,textureScore5, textureScore6,
			textureScore7, textureScore8, textureScore9 };
		static int score = 0;
		static int scoreBoard[3] = { 0,0,0 };  //初始化游戏计分板数组
		int tempScore = 0;		//由于每次都要对score进行一次除法处理，并且score是static的，这导致处理完score后它就变成0了
							//使用这个临时变量，用它来代替score去做除法处理
	//将视角转向游戏开始界面
	if (ViewTurn == 0) {
			//声明一个数组存放颜色信息，4个元素分别表示红，绿，蓝以及alpha
			float ClearColor[4] = { 0.0f, 0.0f, 0.0f, 1.0f };
			immediateContext->ClearRenderTargetView(renderTargetView, ClearColor);
			//清除当前绑定的深度模板视图
			immediateContext->ClearDepthStencilView(depthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);

			//*************设置变换坐标系***********************	
			world = XMMatrixIdentity();
			XMVECTOR Eye = XMVectorSet(35.0f, 5.0f, -2.0f, 0.0f);
			XMVECTOR At = XMVectorSet(35.0f, 5.0f, 0.0f, 0.0f);  //目标位置
			XMVECTOR Up = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);  //up
			view = XMMatrixLookAtLH(Eye, At, Up);   //设置观察坐标系
			//设置投影矩阵
			projection = XMMatrixPerspectiveFovLH(XM_PIDIV2, 800.0f / 600.0f, 0.01f, 100.0f);
			//将坐标变换矩阵的常量缓存中的矩阵和坐标设置到Effect框架中---------------------
			effect->GetVariableByName("World")->AsMatrix()->SetMatrix((float*)&world);  //设置世界坐标系
			effect->GetVariableByName("View")->AsMatrix()->SetMatrix((float*)&view);    //设置观察坐标系
			effect->GetVariableByName("Projection")->AsMatrix()->SetMatrix((float*)&projection); //设置投影坐标系
			effect->GetVariableByName("EyePosition")->AsMatrix()->SetMatrix((float*)&Eye); //设置视点

			//光源的常量缓存中的光源信息设置到Effect框架中---------------------------------
			SetLightEffect(light[lightType]);
			//*************设置变换坐标系***********************

			//::MessageBox(NULL, L"现在时开始界面", L"Error", MB_OK);


			//定义一个D3DX11_TECHNIQUE_DESC对象来描述technique
			D3DX11_TECHNIQUE_DESC techDesc;
			technique->GetDesc(&techDesc);    //获取technique的描述
			//获取通道（PASS）把它设置到设备上下文中。
			//这里由于只有一个通道所以其索引为0


			//swapChain->Present(0, 0);
		}
	//将视角转向游戏界面
	else if (ViewTurn == 1) {
		//声明一个数组存放颜色信息，4个元素分别表示红，绿，蓝以及alpha
		float ClearColor[4] = { 0.0f, 0.0f, 0.0f, 1.0f };
		immediateContext->ClearRenderTargetView(renderTargetView, ClearColor);
		//清除当前绑定的深度模板视图
		immediateContext->ClearDepthStencilView(depthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);

		//*************设置变换坐标系***********************	
		world = XMMatrixIdentity();
		XMVECTOR Eye = XMVectorSet(0.0f, 6.0f, -5.0f, 0.0f);
		XMVECTOR At = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);  //目标位置
		XMVECTOR Up = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);  //up
		view = XMMatrixLookAtLH(Eye, At, Up);   //设置观察坐标系
		//设置投影矩阵
		projection = XMMatrixPerspectiveFovLH(XM_PIDIV2, 800.0f / 600.0f, 0.01f, 100.0f);
		//将坐标变换矩阵的常量缓存中的矩阵和坐标设置到Effect框架中---------------------
		effect->GetVariableByName("World")->AsMatrix()->SetMatrix((float*)&world);  //设置世界坐标系
		effect->GetVariableByName("View")->AsMatrix()->SetMatrix((float*)&view);    //设置观察坐标系
		effect->GetVariableByName("Projection")->AsMatrix()->SetMatrix((float*)&projection); //设置投影坐标系
		effect->GetVariableByName("EyePosition")->AsMatrix()->SetMatrix((float*)&Eye); //设置视点

		//光源的常量缓存中的光源信息设置到Effect框架中---------------------------------
		SetLightEffect(light[lightType]);
		//*************设置变换坐标系***********************

		//::MessageBox(NULL, L"现在时游戏界面", L"Error", MB_OK);

		//定义一个D3DX11_TECHNIQUE_DESC对象来描述technique
		D3DX11_TECHNIQUE_DESC techDesc;
		technique->GetDesc(&techDesc);    //获取technique的描述
		//获取通道（PASS）把它设置到设备上下文中。
		//这里由于只有一个通道所以其索引为0


		//swapChain->Present(0, 0);
	}
	//游戏结束，将视角转向记分板界面
	else {
		//声明一个数组存放颜色信息，4个元素分别表示红，绿，蓝以及alpha
		float ClearColor[4] = { 0.0f, 0.0f, 0.0f, 1.0f };
		immediateContext->ClearRenderTargetView(renderTargetView, ClearColor);
		//清除当前绑定的深度模板视图
		immediateContext->ClearDepthStencilView(depthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);

		//*************设置变换坐标系***********************	
		world = XMMatrixIdentity();
		XMVECTOR Eye = XMVectorSet(35.0f, 6.0f, -2.0f, 0.0f);
		XMVECTOR At = XMVectorSet(35.0f, 5.0f, 0.0f, 0.0f);  //目标位置
		XMVECTOR Up = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);  //up
		view = XMMatrixLookAtLH(Eye, At, Up);   //设置观察坐标系
		//设置投影矩阵
		projection = XMMatrixPerspectiveFovLH(XM_PIDIV2, 800.0f / 600.0f, 0.01f, 100.0f);
		//将坐标变换矩阵的常量缓存中的矩阵和坐标设置到Effect框架中---------------------
		effect->GetVariableByName("World")->AsMatrix()->SetMatrix((float*)&world);  //设置世界坐标系
		effect->GetVariableByName("View")->AsMatrix()->SetMatrix((float*)&view);    //设置观察坐标系
		effect->GetVariableByName("Projection")->AsMatrix()->SetMatrix((float*)&projection); //设置投影坐标系
		effect->GetVariableByName("EyePosition")->AsMatrix()->SetMatrix((float*)&Eye); //设置视点

		//光源的常量缓存中的光源信息设置到Effect框架中---------------------------------
		SetLightEffect(light[lightType]);
		//*************设置变换坐标系***********************

		//::MessageBox(NULL, L"现在是结束界面", L"Error", MB_OK);

		//定义一个D3DX11_TECHNIQUE_DESC对象来描述technique
		D3DX11_TECHNIQUE_DESC techDesc;
		technique->GetDesc(&techDesc);    //获取technique的描述
		//获取通道（PASS）把它设置到设备上下文中。
		//这里由于只有一个通道所以其索引为0


		//swapChain->Present(0, 0);
	}

	//！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！

		float BlendFactor[] = { 0,0,0,0 };

		//********************第一部分：设置3个坐标系及光照的外部变量****************************
		//通过键盘的上下左右键来改变虚拟摄像头方向.
		static float angle = XM_PI;   //声明一个静态变量用于记录角度
		static float height = 2.0f;

		/*OOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO*/
		static int totalTime = 300;     //初始化剩余时间为300秒，初始时间设为300时，游戏里显示299s，不知道为啥
		static int timer[3] = { 0,0,0 };    //初始化游戏时间面板数组

		int GoodMeatTime = 6;     //肉从生肉煎成菜需要的时间
		int BadMeatTime = 10;      //经过多少秒后，肉被煎糊
		if (Cooking == 1) {     //正在煎肉，

			cookingTimer.Tick();  //计时煎肉花费的时间
			if (cookingTimer.TotalTime() > GoodMeatTime) {  //煎肉煎2秒后，肉就煎好了，等着收
				IsMeatCookedWell = 1;

			}
			if (cookingTimer.TotalTime() > BadMeatTime) {   //煎肉煎4秒后，肉就糊了，肉自动消失，无法被收取
				IsMeatCookedWell = 0;
				IsOverCooked = 1;

			}
		}
		/*OOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO*/


		if (::GetAsyncKeyState(VK_LEFT) & 0x8000f) //响应键盘左方向键
			angle -= 1.5f * timeDelta;
		if (::GetAsyncKeyState(VK_RIGHT) & 0x8000f) //响应键盘右方向键
			angle += 1.5f * timeDelta;
		if (::GetAsyncKeyState(VK_UP) & 0x8000f)    //响应键盘上方向键
			height += 4.0f * timeDelta;
		if (::GetAsyncKeyState(VK_DOWN) & 0x8000f)  //响应键盘下方向键
			height -= 4.0f * timeDelta;

		//if (height < -5.0f) height = -5.f;   //限制镜头最远距离
		//if (height > 5.0f) height = 5.f;     //限制镜头最近距离

		//********************第一部分：设置3个坐标系及光照的外部变量****************************

		//********************第二部分：绘制各个物体*************************************************
		//**注意**：绘制多个物体的时候必须先绘制不透明的物体，再绘制透明的物体，
		//          因为后绘制的物体会挡住先绘制的物体，本例中必须按照池子-箱子-水面的顺序绘制

		D3DX11_TECHNIQUE_DESC techDesc;
		technique->GetDesc(&techDesc);    //获取technique的描述

		//绘制地板
		//设置地板的材质信息
		effect->GetVariableByName("MatAmbient")->AsVector()->SetFloatVector((float*)&(floorMaterial.ambient));
		effect->GetVariableByName("MatDiffuse")->AsVector()->SetFloatVector((float*)&(floorMaterial.diffuse));
		effect->GetVariableByName("MatSpecular")->AsVector()->SetFloatVector((float*)&(floorMaterial.specular));
		effect->GetVariableByName("MatPower")->AsScalar()->SetFloat(floorMaterial.power);
		//设置地板的纹理
		effect->GetVariableByName("Texture")->AsShaderResource()->SetResource(textureFloor);
		technique->GetPassByIndex(0)->Apply(0, immediateContext);
		immediateContext->Draw(6, 150);

		if (flag < 300) {
			//设置传送带纹理
			effect->GetVariableByName("Texture")->AsShaderResource()->SetResource(textureScrollLine);
			technique->GetPassByIndex(0)->Apply(0, immediateContext);
			immediateContext->Draw(6, 36);//绘制传送带
			flag++;
		}
		else if (flag < 600) {
			effect->GetVariableByName("Texture")->AsShaderResource()->SetResource(textureScrollLine);
			technique->GetPassByIndex(0)->Apply(0, immediateContext);
			immediateContext->Draw(6, 228);//绘制传送带
			flag++;
		}
		else if (flag < 900) {
			effect->GetVariableByName("Texture")->AsShaderResource()->SetResource(textureScrollLine);
			technique->GetPassByIndex(0)->Apply(0, immediateContext);
			immediateContext->Draw(6, 234);//绘制传送带
			flag++;
		}
		else flag = 0;

		//绘制箱子
		//设置箱子的材质信息
		effect->GetVariableByName("MatAmbient")->AsVector()->SetFloatVector((float*)&(boxMaterial.ambient));
		effect->GetVariableByName("MatDiffuse")->AsVector()->SetFloatVector((float*)&(boxMaterial.diffuse));
		effect->GetVariableByName("MatSpecular")->AsVector()->SetFloatVector((float*)&(boxMaterial.specular));
		effect->GetVariableByName("MatPower")->AsScalar()->SetFloat(boxMaterial.power);
		//设置箱子的纹理
		effect->GetVariableByName("Texture")->AsShaderResource()->SetResource(textureBox);
		technique->GetPassByIndex(0)->Apply(0, immediateContext);
		immediateContext->Draw(66, 0);   //绘制箱子

		/*OOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO*/

		//只有游戏在进行中，才可以计分
		if (!GameOver && GameStart)
			if (IsGetScore) {//检测到能够得分，分数增长
				score = score + RewardScore;
				//::MessageBox(NULL, stringToLPCWSTR(Convert(score)), L"Over", MB_OK);     //时间倒计时结束，弹窗
				IsGetScore = false;
			}
		tempScore = score;

		//绘制面板
		if (!GameStart && !GameOver) {   //刚进入游戏时，start与GameOver都是false，此时仅绘制开始界面
			//还未开始游戏时，在这里绘制游戏初始界面
			world = world * XMMatrixTranslation(35.7f, 1.5f, 0.0f);
			effect->GetVariableByName("World")->AsMatrix()->SetMatrix((float*)&world);
			effect->GetVariableByName("Texture")->AsShaderResource()->SetResource(startGame);
			technique->GetPassByIndex(0)->Apply(0, immediateContext);
			immediateContext->Draw(6, 354);
			world = XMMatrixIdentity();
			effect->GetVariableByName("World")->AsMatrix()->SetMatrix((float*)&world);  //设置世界坐标系
		}
		else if (GameStart && GameOver) {                 //时间结束后，绘制结束界面，同时也将分数面板绘制上去（即此时仅有结束界面和分数面板）
			ViewTurn = 3;
			//计算分数面板该显示的数字纹理
			for (int i = 0; i < 3; i++) {
				scoreBoard[i] = tempScore % 10;
				tempScore /= 10;
			}

			//----------------------------------------------------------------------------------分数面板
			world = world * XMMatrixTranslation(35.3f, 0.0f, 0.0f);
			effect->GetVariableByName("World")->AsMatrix()->SetMatrix((float*)&world);
			effect->GetVariableByName("Texture")->AsShaderResource()->SetResource(textureNumber[scoreBoard[2]]);
			technique->GetPassByIndex(0)->Apply(0, immediateContext);
			immediateContext->Draw(6, 348);

			world = world * XMMatrixTranslation(0.25f, 0.0f, 0.0f);
			effect->GetVariableByName("World")->AsMatrix()->SetMatrix((float*)&world);
			effect->GetVariableByName("Texture")->AsShaderResource()->SetResource(textureNumber[scoreBoard[1]]);
			technique->GetPassByIndex(0)->Apply(0, immediateContext);
			immediateContext->Draw(6, 348);

			world = world * XMMatrixTranslation(0.25f, 0.0f, 0.0f);
			effect->GetVariableByName("World")->AsMatrix()->SetMatrix((float*)&world);
			effect->GetVariableByName("Texture")->AsShaderResource()->SetResource(textureNumber[scoreBoard[0]]);
			technique->GetPassByIndex(0)->Apply(0, immediateContext);
			immediateContext->Draw(6, 348);

			world = XMMatrixIdentity();
			effect->GetVariableByName("World")->AsMatrix()->SetMatrix((float*)&world);  //设置世界坐标系

		}
		else {                                                         //游戏进行中，绘制分数面板和定时面板
			mytimer.Tick();  //每绘制一帧，定时器里增加响应的时间

			int leftTime = totalTime - mytimer.TotalTime();
			if (leftTime < 0) {
				//start = false;          //时间到，start标志改为false
				leftTime = 0;
				GameOver = true;
			}

			//计算时间面板该显示的数字纹理
			for (int i = 0; i < 3; i++) {
				timer[i] = leftTime % 10;
				leftTime /= 10;
			}

			//计算分数面板该显示的数字纹理
			for (int i = 0; i < 3; i++) {
				scoreBoard[i] = tempScore % 10;
				tempScore /= 10;
			}

			//--------------------------------------------------- 分数面板
			{
				world = world * XMMatrixTranslation(0.3f, 0.0f, 0.0f);
				effect->GetVariableByName("World")->AsMatrix()->SetMatrix((float*)&world);
				effect->GetVariableByName("Texture")->AsShaderResource()->SetResource(textureNumber[scoreBoard[2]]);
				technique->GetPassByIndex(0)->Apply(0, immediateContext);
				immediateContext->Draw(6, 348);

				world = world * XMMatrixTranslation(0.25f, 0.0f, 0.0f);
				effect->GetVariableByName("World")->AsMatrix()->SetMatrix((float*)&world);
				effect->GetVariableByName("Texture")->AsShaderResource()->SetResource(textureNumber[scoreBoard[1]]);
				technique->GetPassByIndex(0)->Apply(0, immediateContext);
				immediateContext->Draw(6, 348);

				world = world * XMMatrixTranslation(0.25f, 0.0f, 0.0f);
				effect->GetVariableByName("World")->AsMatrix()->SetMatrix((float*)&world);
				effect->GetVariableByName("Texture")->AsShaderResource()->SetResource(textureNumber[scoreBoard[0]]);
				technique->GetPassByIndex(0)->Apply(0, immediateContext);
				immediateContext->Draw(6, 348);
			}

			//--------------------------------------------------- 时间面板
			{
				world = world * XMMatrixTranslation(3.3f, 0.0f, 0.0f);
				effect->GetVariableByName("World")->AsMatrix()->SetMatrix((float*)&world);
				effect->GetVariableByName("Texture")->AsShaderResource()->SetResource(textureNumber[timer[2]]);
				technique->GetPassByIndex(0)->Apply(0, immediateContext);
				immediateContext->Draw(6, 342);

				world = world * XMMatrixTranslation(0.25f, 0.0f, 0.0f);
				effect->GetVariableByName("World")->AsMatrix()->SetMatrix((float*)&world);
				effect->GetVariableByName("Texture")->AsShaderResource()->SetResource(textureNumber[timer[1]]);
				technique->GetPassByIndex(0)->Apply(0, immediateContext);
				immediateContext->Draw(6, 342);

				world = world * XMMatrixTranslation(0.25f, 0.0f, 0.0f);
				effect->GetVariableByName("World")->AsMatrix()->SetMatrix((float*)&world);
				effect->GetVariableByName("Texture")->AsShaderResource()->SetResource(textureNumber[timer[0]]);
				technique->GetPassByIndex(0)->Apply(0, immediateContext);
				immediateContext->Draw(6, 342);

				world = XMMatrixIdentity();
				effect->GetVariableByName("World")->AsMatrix()->SetMatrix((float*)&world);  //设置世界坐标系
			}


		}
		/*OOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO*/


		//盘子
		effect->GetVariableByName("Texture")->AsShaderResource()->SetResource(texturePlatebread);
		technique->GetPassByIndex(0)->Apply(0, immediateContext);
		immediateContext->Draw(6, 240);
		effect->GetVariableByName("Texture")->AsShaderResource()->SetResource(texturePlatemeat);
		technique->GetPassByIndex(0)->Apply(0, immediateContext);
		immediateContext->Draw(6, 246);
		effect->GetVariableByName("Texture")->AsShaderResource()->SetResource(texturePlatetomato);
		technique->GetPassByIndex(0)->Apply(0, immediateContext);
		immediateContext->Draw(6, 252);
		effect->GetVariableByName("Texture")->AsShaderResource()->SetResource(texturePlatevegetable);
		technique->GetPassByIndex(0)->Apply(0, immediateContext);
		immediateContext->Draw(6, 258);

		//垃圾箱
		effect->GetVariableByName("Texture")->AsShaderResource()->SetResource(textureRubbishl);
		technique->GetPassByIndex(0)->Apply(0, immediateContext);
		immediateContext->Draw(6, 264);
		effect->GetVariableByName("Texture")->AsShaderResource()->SetResource(textureRubbishr);
		technique->GetPassByIndex(0)->Apply(0, immediateContext);
		immediateContext->Draw(6, 270);
		//锅
		if (IsMeatCookedWell == 1)
			effect->GetVariableByName("Texture")->AsShaderResource()->SetResource(textureCookedWell);
		else if (IsOverCooked == 1)
			effect->GetVariableByName("Texture")->AsShaderResource()->SetResource(textureOverCooked);
		else if (Cooking == 1)
			effect->GetVariableByName("Texture")->AsShaderResource()->SetResource(textureCooking);
		else
			effect->GetVariableByName("Texture")->AsShaderResource()->SetResource(texturePot);
		technique->GetPassByIndex(0)->Apply(0, immediateContext);
		immediateContext->Draw(6, 276);
		//菜板
		effect->GetVariableByName("Texture")->AsShaderResource()->SetResource(textureCuttingboard);
		technique->GetPassByIndex(0)->Apply(0, immediateContext);
		immediateContext->Draw(6, 282);
		//出口
		effect->GetVariableByName("Texture")->AsShaderResource()->SetResource(textureOut);
		technique->GetPassByIndex(0)->Apply(0, immediateContext);
		immediateContext->Draw(6, 288);


		//抽屉
		effect->GetVariableByName("MatAmbient")->AsVector()->SetFloatVector((float*)&(waterMaterial.ambient));
		effect->GetVariableByName("MatDiffuse")->AsVector()->SetFloatVector((float*)&(waterMaterial.diffuse));
		effect->GetVariableByName("MatSpecular")->AsVector()->SetFloatVector((float*)&(waterMaterial.specular));
		effect->GetVariableByName("MatPower")->AsScalar()->SetFloat(waterMaterial.power);

		//设置抽屉的纹理
		effect->GetVariableByName("Texture")->AsShaderResource()->SetResource(textureDrawer);
		technique->GetPassByIndex(0)->Apply(0, immediateContext);
		immediateContext->Draw(84, 66);   //绘制抽屉

		//设置墙纹理
		effect->GetVariableByName("Texture")->AsShaderResource()->SetResource(textureWall);
		technique->GetPassByIndex(0)->Apply(0, immediateContext);
		immediateContext->Draw(30, 156);//绘制墙

		//设置墙顶部纹理
		effect->GetVariableByName("Texture")->AsShaderResource()->SetResource(textureWallTop);
		technique->GetPassByIndex(0)->Apply(0, immediateContext);
		immediateContext->Draw(30, 186);//绘制墙顶部

		//绘制垃圾桶
		effect->GetVariableByName("Texture")->AsShaderResource()->SetResource(textureRubbishl);
		technique->GetPassByIndex(0)->Apply(0, immediateContext);
		immediateContext->Draw(6, 312);   //绘制垃圾桶

		//菜单绘制
		if (menuFlag == 1) {
			createmenu(menunum);
		}
		//----------------------------------------------------------------------------------WASD键控制玩家一移动
		static int walkX = 0;     //用来控制选取一行的哪张图片作为当前纹理
		static int actSpeed = 0;  //控制人物模型行走动作的速度
		static float moveX = 0.0f; //X方向位移量
		static float moveZ = 0.0f; //Y方向位移量
		if (::GetAsyncKeyState('W') & 0x8000f) {    //响应键盘上方向键
			if (walkX > 5)	walkX = 0;
			if (actSpeed % 3 == 0) {
				vertices[216].Tex = XMFLOAT2(0.167f * walkX, 0.0f);
				vertices[217].Tex = XMFLOAT2(0.167f * (walkX + 1), 0.0f);
				vertices[218].Tex = XMFLOAT2(0.167f * walkX, 0.25f);

				vertices[219].Tex = XMFLOAT2(0.167f * walkX, 0.25f);
				vertices[220].Tex = XMFLOAT2(0.167f * (walkX + 1), 0.0f);
				vertices[221].Tex = XMFLOAT2(0.167f * (walkX + 1), 0.25f);
				walkX++;
			}
			moveZ += 2.0f * timeDelta;

			actSpeed++;
		}
		if (::GetAsyncKeyState('S') & 0x8000f) { //S键向下走
			if (walkX > 5)walkX = 0;
			if (actSpeed % 3 == 0) {
				vertices[216].Tex = XMFLOAT2(0.167f * walkX, 0.25f);
				vertices[217].Tex = XMFLOAT2(0.167f * (walkX + 1), 0.25f);
				vertices[218].Tex = XMFLOAT2(0.167f * walkX, 0.5f);

				vertices[219].Tex = XMFLOAT2(0.167f * walkX, 0.5f);
				vertices[220].Tex = XMFLOAT2(0.167f * (walkX + 1), 0.25f);
				vertices[221].Tex = XMFLOAT2(0.167f * (walkX + 1), 0.5f);
				walkX++;
			}
			actSpeed++;
			moveZ -= 2.0f * timeDelta;
		}
		if (::GetAsyncKeyState('A') & 0x8000f) {    //A键向左走
			if (walkX > 5)walkX = 0;
			vertices[216].Tex = XMFLOAT2(0.167f * walkX, 0.75f);
			vertices[217].Tex = XMFLOAT2(0.167f * (walkX + 1), 0.75f);
			vertices[218].Tex = XMFLOAT2(0.167f * walkX, 1.0f);

			vertices[219].Tex = XMFLOAT2(0.167f * walkX, 1.0f);
			vertices[220].Tex = XMFLOAT2(0.167f * (walkX + 1), 0.75f);
			vertices[221].Tex = XMFLOAT2(0.167f * (walkX + 1), 1.0f);

			walkX++;
			moveX -= 2.0f * timeDelta;
		}
		if (::GetAsyncKeyState('D') & 0x8000f) {  //D键向右走
			if (walkX > 5)walkX = 0;
			vertices[216].Tex = XMFLOAT2(0.167f * walkX, 0.5f);
			vertices[217].Tex = XMFLOAT2(0.167f * (walkX + 1), 0.5f);
			vertices[218].Tex = XMFLOAT2(0.167f * walkX, 0.75f);

			vertices[219].Tex = XMFLOAT2(0.167f * walkX, 0.75f);
			vertices[220].Tex = XMFLOAT2(0.167f * (walkX + 1), 0.5f);
			vertices[221].Tex = XMFLOAT2(0.167f * (walkX + 1), 0.75f);
			walkX++;
			moveX += 2.0f * timeDelta;
		}

		if (moveZ <= 0.0f) {
			if (moveZ < -3.0f)moveZ = -2.9f;  //解决下，左，右三个大边界
			if (moveX > 5.0f)moveX = 4.8f;
			if (moveX < -7.0f)moveX = -6.8f;

			if (moveX > -4.8f && moveX < -1.3f && moveZ>-0.2f)moveZ = -0.2f;
			if (moveX > 1.8f && moveX < 5.2f && moveZ>-0.2f)moveZ = -0.2f;
		}

		if (moveZ > 0.0f) {
			if (moveZ > 0.9f)moveZ = 0.9f;
			if (moveX > 2.0f)moveX = 2.0f;
			if (moveX < -1.0f && moveX > -2.0f)moveX = -1.0f;
			if (moveX > -5.0f && moveX < -2.0f)moveX = -5.0f;
			if (moveX < -7.0f)moveX = -7.0f;
		}
		//----------------------------------------------------------------------------------WASD键控制玩家一移动

		//----------------------------------------------------------------------------------IJKL键控制玩家2移动
		static int walkX2 = 0;     //用来控制选取一行的哪张图片作为当前纹理
		static int actSpeed2 = 0;  //控制人物模型行走动作的速度
		static float moveX2 = 0.0f; //X方向位移量
		static float moveZ2 = 0.0f; //Y方向位移量
		if (::GetAsyncKeyState(VK_UP) & 0x8000f) {    //响应键盘上方向键
			if (walkX2 > 5)walkX2 = 0;
			if (actSpeed2 % 3 == 0) {
				vertices[222].Tex = XMFLOAT2(0.167f * walkX2, 0.0f);
				vertices[223].Tex = XMFLOAT2(0.167f * (walkX2 + 1), 0.0f);
				vertices[224].Tex = XMFLOAT2(0.167f * walkX2, 0.25f);

				vertices[225].Tex = XMFLOAT2(0.167f * walkX2, 0.25f);
				vertices[226].Tex = XMFLOAT2(0.167f * (walkX2 + 1), 0.0f);
				vertices[227].Tex = XMFLOAT2(0.167f * (walkX2 + 1), 0.25f);
				walkX2++;
			}
			moveZ2 += 2.0f * timeDelta;

			actSpeed2++;
		}
		if (::GetAsyncKeyState(VK_DOWN) & 0x8000f) { //下方向键向下走
			if (walkX2 > 5)walkX2 = 0;
			if (actSpeed2 % 3 == 0) {
				vertices[222].Tex = XMFLOAT2(0.167f * walkX2, 0.25f);
				vertices[223].Tex = XMFLOAT2(0.167f * (walkX2 + 1), 0.25f);
				vertices[224].Tex = XMFLOAT2(0.167f * walkX2, 0.5f);

				vertices[225].Tex = XMFLOAT2(0.167f * walkX2, 0.5f);
				vertices[226].Tex = XMFLOAT2(0.167f * (walkX2 + 1), 0.25f);
				vertices[227].Tex = XMFLOAT2(0.167f * (walkX2 + 1), 0.5f);
				walkX2++;
			}
			actSpeed2++;
			moveZ2 -= 2.0f * timeDelta;
		}
		if (::GetAsyncKeyState(VK_RIGHT) & 0x8000f) {    //右方向键向左走
			if (walkX2 > 5)walkX2 = 0;
			vertices[222].Tex = XMFLOAT2(0.167f * walkX2, 0.75f);
			vertices[223].Tex = XMFLOAT2(0.167f * (walkX2 + 1), 0.75f);
			vertices[224].Tex = XMFLOAT2(0.167f * walkX2, 1.0f);

			vertices[225].Tex = XMFLOAT2(0.167f * walkX2, 1.0f);
			vertices[226].Tex = XMFLOAT2(0.167f * (walkX2 + 1), 0.75f);
			vertices[227].Tex = XMFLOAT2(0.167f * (walkX2 + 1), 1.0f);

			walkX2++;
			moveX2 -= 2.0f * timeDelta;
		}
		if (::GetAsyncKeyState(VK_LEFT) & 0x8000f) {  //左方向键向右走
			if (walkX2 > 5)walkX2 = 0;
			vertices[222].Tex = XMFLOAT2(0.167f * walkX2, 0.5f);
			vertices[223].Tex = XMFLOAT2(0.167f * (walkX2 + 1), 0.5f);
			vertices[224].Tex = XMFLOAT2(0.167f * walkX2, 0.75f);

			vertices[225].Tex = XMFLOAT2(0.167f * walkX2, 0.75f);
			vertices[226].Tex = XMFLOAT2(0.167f * (walkX2 + 1), 0.5f);
			vertices[227].Tex = XMFLOAT2(0.167f * (walkX2 + 1), 0.75f);
			walkX2++;
			moveX2 += 2.0f * timeDelta;
		}

		/*==================碰撞检测*/
		if (moveZ2 >= 0.0f) {
			if (moveZ2 > 2.8f)	moveZ2 = 2.8f;  //解决下，左，右三个大边界
			if (moveX2 > 4.0f)	moveX2 = 3.8f;
			if (moveX2 < -8.0f)	moveX2 = -7.8f;

			if (moveX2 > -5.8f && moveX2 < -2.3f && moveZ2 < 0.2f)	moveZ2 = 0.2f;
			if (moveX2 > 1.1f && moveX2 < 4.2f && moveZ2 < 0.2f)	moveZ2 = 0.2f;
		}

		if (moveZ2 < 0.0f) {
			if (moveZ2 < -0.9f)	moveZ2 = -0.9f;
			if (moveX2 > 1.1f)	moveX2 = 1.1f;

			if (moveX2 < -2.0f && moveX2 > -3.0f)	moveX2 = -2.0f;

			if (moveX2 > -5.8f && moveX2 < -3.0f)	moveX2 = -5.8f;
			if (moveX2 < -8.0f)	moveX2 = -8.0f;
		}

		//----------------------------------------------------------------------------------IJKL键控制玩家2移动

		//-----------------------------------------------------------------------------------重新加载纹理缓存
		//声明HRESULT的对象用于记录函数调用是否成功
		hr = S_OK;
		vertexCount = ARRAYSIZE(vertices);
		//创建顶点缓存，方法同实验4一样
		ZeroMemory(&bd, sizeof(bd));
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.ByteWidth = sizeof(Vertex) * vertexCount;
		bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;  //注意：这里表示创建的是顶点缓存
		bd.CPUAccessFlags = 0;
		ZeroMemory(&InitData, sizeof(InitData));
		InitData.pSysMem = vertices;         //设置需要初始化的数据，这里的数据就是顶点数组
		//调用CreateBuffer创建顶点缓存
		if (DrawTime > 0)
			vertexBuffer->Release();
		hr = device->CreateBuffer(&bd, &InitData, &vertexBuffer);

		if (FAILED(hr))
		{
			::MessageBox(NULL, L"创建VertexBuffer失败", L"Error", MB_OK);
			return hr;
		}

		UINT stride = sizeof(Vertex);                 //获取Vertex的大小作为跨度
		UINT offset = 0;                              //设置偏移量为0
		//设置顶点缓存，参数的解释见实验4
		immediateContext->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);
		DrawTime = 1;
		//---------------------------------------------------------------------------------------重新加载纹理缓存

		world = XMMatrixIdentity();
		world = XMMatrixTranslation(moveX, 0.0f, moveZ);
		effect->GetVariableByName("World")->AsMatrix()->SetMatrix((float*)&world);  //设置世界坐标系

		immediateContext->OMSetBlendState(blendStateAlpha, BlendFactor, 0xffffffff);  //开启混合
		immediateContext->RSSetState(noCullRS);                                       //关闭背面消隐

		//world = XMMatrixIdentity();
		effect->GetVariableByName("MatAmbient")->AsVector()->SetFloatVector((float*)&(waterMaterial.ambient));
		effect->GetVariableByName("MatDiffuse")->AsVector()->SetFloatVector((float*)&(waterMaterial.diffuse));
		effect->GetVariableByName("MatSpecular")->AsVector()->SetFloatVector((float*)&(waterMaterial.specular));
		effect->GetVariableByName("MatPower")->AsScalar()->SetFloat(waterMaterial.power);
		//设置人物1的纹理
		effect->GetVariableByName("Texture")->AsShaderResource()->SetResource(texturePlayer1);
		technique->GetPassByIndex(0)->Apply(0, immediateContext);
		immediateContext->Draw(6, 216);

		//接下来是互动判断
		{

			//空格键的互动判断（按下如果有且在边界，则放下，如果无，则判断位置，满足条件可取物）
			if (::GetAsyncKeyState(VK_SPACE) & 0x8000f)
			{
				CanCookMeat(moveX, moveZ);
				CanGetFood(moveX, moveZ);
				CanPutInScroll(moveX, moveZ);
				CanThrowFood(moveX, moveZ);
			}
			//Enter的互动判断（按下如果有且在边界，则放下，如果无，则判断位置，满足条件可取物）
			//判断的是上侧人物的取物放物
			if (::GetAsyncKeyState(VK_RETURN) & 0x8000f)
			{
				CanGetFromScroll(moveX2, moveZ2);
				CanCutVegtable(moveX2, moveZ2);
				SendOutFood(moveX2, moveZ2);
			}

		}
		//互动判断结束

		//互动之后的纹理改变以及运动
		//指示下面的人头顶上是否应该显示煎好的肉
		if (IsMeatWell[0] == 1)
			setTextureOfPlayerHead(300, moveX, moveZ, textureMeatCooked);
		//指示上面的人头顶上是否应该显示切好的西红柿
		if (IsTomatoWell[1] == 1)
			setTextureOfPlayerHead(306, -moveX2, moveZ2, textureTomatoCutted);
		//指示上面的人头顶上是否应该显示切好的生菜
		if (IsLettuceWell[1] == 1)
			setTextureOfPlayerHead(306, -moveX2, moveZ2, textureLettuceCutted);
		//指示上面的人头顶上是否应该显示切好的面包
		if (IsBreadWell[1] == 1)
			setTextureOfPlayerHead(306, -moveX2, moveZ2, textureBreadCutted);
		//只是上面的人头顶上是否应该显示煎好的肉
		if (IsMeatWell[1] == 1)
			setTextureOfPlayerHead(306, -moveX2, moveZ2, textureMeatCooked);
		//取到面包
		if (IsBread[0] == 1)
			setTextureOfPlayerHead(300, moveX, moveZ, textureHeadbread);
		//取到肉
		if (IsMeat[0] == 1)
			setTextureOfPlayerHead(300, moveX, moveZ, textureHeadmeat);
		//取到西红柿
		if (IsTomato[0] == 1)
			setTextureOfPlayerHead(300, moveX, moveZ, textureHeadtomato);
		//取到蔬菜
		if (IsLettuce[0] == 1)
			setTextureOfPlayerHead(300, moveX, moveZ, textureHeadvegetable);
		//取到烤糊的肉
		if (IsHaveOverCookedMeat == 1)
			setTextureOfPlayerHead(300, moveX, moveZ, textureOverCookedMeat);

		if (IsBread[1] == 1)
			setTextureOfPlayerHead(306, -moveX2, moveZ2, textureHeadbread);
		if (IsLettuce[1] == 1)
			setTextureOfPlayerHead(306, -moveX2, moveZ2, textureHeadvegetable);
		if (IsTomato[1] == 1)
			setTextureOfPlayerHead(306, -moveX2, moveZ2, textureHeadtomato);
		//面包放到传送带上
		if (IsBreadInSC == 1)
			putFoodInScroll(1, PutToSCXMove, 7.0f, timeDelta, 300, textureHeadbread);
		//肉放到传送带上
		if (IsMeatWellInSC == 1)
			putFoodInScroll(1, PutToSCXMove, 7.0f, timeDelta, 300, textureMeatCooked);
		//西红柿放到传送带上
		if (IsTomatoInSC == 1)
			putFoodInScroll(1, PutToSCXMove, 7.0f, timeDelta, 300, textureHeadtomato);
		//蔬菜放到传送带上
		if (IsLettuceInSC == 1)
			putFoodInScroll(1, PutToSCXMove, 7.0f, timeDelta, 300, textureHeadvegetable);

		/*************************************************************/
		world = XMMatrixIdentity();
		world = XMMatrixTranslation(-moveX2, 0.0f, moveZ2);
		effect->GetVariableByName("World")->AsMatrix()->SetMatrix((float*)&world);  //设置世界坐标系
		effect->GetVariableByName("MatAmbient")->AsVector()->SetFloatVector((float*)&(waterMaterial.ambient));
		effect->GetVariableByName("MatDiffuse")->AsVector()->SetFloatVector((float*)&(waterMaterial.diffuse));
		effect->GetVariableByName("MatSpecular")->AsVector()->SetFloatVector((float*)&(waterMaterial.specular));
		effect->GetVariableByName("MatPower")->AsScalar()->SetFloat(waterMaterial.power);

		//设置人物2的纹理
		effect->GetVariableByName("Texture")->AsShaderResource()->SetResource(texturePlayer2);
		technique->GetPassByIndex(0)->Apply(0, immediateContext);
		immediateContext->Draw(6, 222);
		immediateContext->OMSetBlendState(0, 0, 0xffffffff);   //关闭混合，记住最后必须关闭混合
		immediateContext->RSSetState(0);                     //恢复背面消隐
		//********************第二部分：绘制各个物体*************************************************

		swapChain->Present(0, 0);
	}
	return true;
}
//**************框架函数******************

// 在人物当前坐标处把食物放置到传送带上
void putFoodInScroll(int playerNum, float startPos, float stepLen, float timeDelta, int vertexNums, ID3D11ShaderResourceView* textureTemp)
{
	world = XMMatrixIdentity();
	if (PutToSCXMove < -7.0f)
		PutToSCXMove = -100.0f;
	PutToSCXMove -= 0.3 * timeDelta;
	if (PutToSCXMove < -stepLen)
	{
		IsBreadInSC = 0;
		IsMeatWellInSC = 0;
		IsTomatoInSC = 0;
		IsLettuceInSC = 0;
	}
	world = XMMatrixTranslation(PutToSCXMove, 0.0f, 0.0f);
	effect->GetVariableByName("World")->AsMatrix()->SetMatrix((float*)&world);  //设置世界坐标系
	effect->GetVariableByName("MatAmbient")->AsVector()->SetFloatVector((float*)&(waterMaterial.ambient));
	effect->GetVariableByName("MatDiffuse")->AsVector()->SetFloatVector((float*)&(waterMaterial.diffuse));
	effect->GetVariableByName("MatSpecular")->AsVector()->SetFloatVector((float*)&(waterMaterial.specular));
	effect->GetVariableByName("MatPower")->AsScalar()->SetFloat(waterMaterial.power);

	effect->GetVariableByName("Texture")->AsShaderResource()->SetResource(textureTemp);
	technique->GetPassByIndex(0)->Apply(0, immediateContext);
	immediateContext->Draw(6, vertexNums);
}

//改变人物头顶盘子的纹理
void setTextureOfPlayerHead(int vertexNums, float tmpMoveX, float tmpMoveZ, ID3D11ShaderResourceView* textureTempChange)
{
	world = XMMatrixIdentity();
	world = XMMatrixTranslation(tmpMoveX, 0.0f, tmpMoveZ);
	effect->GetVariableByName("World")->AsMatrix()->SetMatrix((float*)&world);  //设置世界坐标系 

	effect->GetVariableByName("MatAmbient")->AsVector()->SetFloatVector((float*)&(waterMaterial.ambient));
	effect->GetVariableByName("MatDiffuse")->AsVector()->SetFloatVector((float*)&(waterMaterial.diffuse));
	effect->GetVariableByName("MatSpecular")->AsVector()->SetFloatVector((float*)&(waterMaterial.specular));
	effect->GetVariableByName("MatPower")->AsScalar()->SetFloat(waterMaterial.power);
	effect->GetVariableByName("Texture")->AsShaderResource()->SetResource(textureTempChange);
	technique->GetPassByIndex(0)->Apply(0, immediateContext);
	immediateContext->Draw(6, vertexNums);
	//MessageBox(NULL, L"Get1111111111!", L"get food OK", MB_OK);
}

//判断人物是否到达取材地点
bool CanGetFood(float x, float z)
{
	if (IsBread[0] == 0 && IsTomato[0] == 0 && IsMeat[0] == 0 && IsLettuce[0] == 0 && IsBreadWell[0] == 0 && IsTomatoWell[0] == 0 && IsMeatWell[0] == 0 && IsLettuceWell[0] == 0 && IsHaveOverCookedMeat == 0)
		//判断下方人物的获取逻辑
	{
		//从箱子获取食物
		if (z <= -2.7f)
		{
			if (x > 0.8f && x < 1.2f)
			{
				IsBread[0] = 1;
				texturePlayer1GetFood = texturePlatebread;
				getFoodPlayer1VertexNums = 240;
				return true;
			}
			else if (x > 1.8f && x < 2.4f)
			{
				IsMeat[0] = 1;
				texturePlayer1GetFood = texturePlatemeat;
				getFoodPlayer1VertexNums = 246;
				return true;
			}
			else if (x > 3.0f && x < 3.4f)
			{
				IsTomato[0] = 1;
				texturePlayer1GetFood = texturePlatetomato;
				getFoodPlayer1VertexNums = 252;
				return true;
			}
			else if (x > 3.8f && x < 4.2f)
			{
				IsLettuce[0] = 1;
				texturePlayer1GetFood = texturePlatevegetable;
				getFoodPlayer1VertexNums = 258;
				return true;
			}
		}
		//从煎锅获取食物
		else if (z >= -0.8 && z <= 0.8)
		{
			if (x <= -6.8f)
			{
				if (Cooking == 1)
				{
					if (IsMeatCookedWell == 1)
					{
						IsMeatWell[0] = 1;
						Cooking = 0;
						IsMeatCookedWell = 0;
						cookingTimer.Stop();    //肉被取走后，取肉定时器停止工作
						return true;
					}
					if (IsOverCooked == 1)
					{
						Cooking = 0;
						IsMeatCookedWell = 0;
						IsOverCooked = 0;
						IsHaveOverCookedMeat = 1;
						cookingTimer.Stop();    //肉被取走后，取肉定时器停止工作
					}
				}
			}
		}
	}
	//判断上方人物的获取逻辑
	return false;
}

bool CanCookMeat(float x, float z)
{
	if (z >= -0.8f && z <= -0.3f)
		if (x <= -6.7)
		{
			if (IsMeat[0] == 1 && Cooking == 0)
			{
				Cooking = 1;
				/*       下面这行放到开始煎肉（人物1把肉放到锅里的时候）的判定里        */
				cookingTimer.Reset();//可以煎肉了，则煎肉计时器开始计时
			/*       上面这行放到开始煎肉（人物1把肉放到锅里的时候）的判定里        */
				IsMeat[0] = 0;
				return true;
			}
		}
	return false;
}

bool CanCutVegtable(float x, float z)
{
	if (IsTomato[1] == 1 || IsLettuce[1] == 1 || IsBread[1] == 1)
	{
		if (z > 2.7f)
		{
			if (x >= -1.3f && x <= -0.7f)
			{
				if (Cutting == 0)
				{
					Cutting = 1;
					//等待一段时候或者不等待//或者限制移动
					IsTomatoWell[1] = IsTomato[1];
					IsBreadWell[1] = IsBread[1];
					IsLettuceWell[1] = IsLettuce[1];
					IsTomato[1] = 0;
					IsBread[1] = 0;
					IsLettuce[1] = 0;
					Cutting = 0;
					return true;
				}
			}
		}
		return false;
	}
	return false;
}

bool CanPutInScroll(float x, float z)
{
	if (IsTomato[0] == 1 || IsLettuce[0] == 1 || IsBread[0] == 1 || IsMeatWell[0] == 1)
	{
		if (z > 0.8)
		{
			IsTomatoInSC = IsTomato[0];
			IsMeatWellInSC = IsMeatWell[0];
			IsBreadInSC = IsBread[0];
			IsLettuceInSC = IsLettuce[0];
			IsTomato[0] = 0;
			IsLettuce[0] = 0;
			IsBread[0] = 0;
			IsMeatWell[0] = 0;
			PutToSCXMove = x;
			return true;
		}
	}
	return false;
}

bool CanGetFromScroll(float x, float z)
{
	if (IsBread[1] == 0 && IsTomato[1] == 0 && IsMeat[1] == 0 && IsLettuce[1] == 0 && IsBreadWell[1] == 0 && IsTomatoWell[1] == 0 && IsMeatWell[1] == 0 && IsLettuceWell[1] == 0)
	{
		if (z <= -0.8 && x <= -PutToSCXMove - 2.0f && x >= -PutToSCXMove - 3.0f)//获取传送带上的菜
		{
			if (IsTomatoInSC == 1 || IsBreadInSC == 1 || IsLettuceInSC == 1 || IsMeatWellInSC == 1)
			{
				IsTomato[1] = IsTomatoInSC;
				IsLettuce[1] = IsLettuceInSC;
				IsBread[1] = IsBreadInSC;
				IsMeatWell[1] = IsMeatWellInSC;
				IsTomatoInSC = 0;
				IsLettuceInSC = 0;
				IsBreadInSC = 0;
				IsMeatWellInSC = 0;
				return true;
			}
		}
	}
	return false;
}

bool SendOutFood(float x, float z)
{
	if (z <= 2.8f && z >= 1.2f && x <= -7.7f)
	{
		if (IsTomatoWell[1] == 1)
		{
			FoodVarity[2] += 1;
			IsTomatoWell[1] = 0;
		}
		else if (IsBreadWell[1] == 1)
		{
			FoodVarity[0] += 1;
			IsBreadWell[1] = 0;
		}
		else if (IsMeatWell[1] == 1)
		{
			FoodVarity[3] += 1;
			IsMeatWell[1] = 0;
		}
		else if (IsLettuceWell[1] == 1)
		{
			FoodVarity[1] += 1;
			IsLettuceWell[1] = 0;
		}
	}
	if (MenuKind == 0)//面包，生菜，番茄
	{
		if (FoodVarity[0] >= 1 && FoodVarity[1] >= 1 && FoodVarity[2] >= 1)
		{
			for (int i = 0; i < 3; i++)
				FoodVarity[i] -= 1;
			//MessageBox(NULL, L"输出菜品", L"YES", MB_OK);
			PlaySound(L"music/FinishCooking.wav", NULL, SND_FILENAME | SND_ASYNC);
			IsGetScore = true;
			srand((int)time(0));
			menunum = rand() % 999;
			return true;
		}
	}
	if (MenuKind == 1)//面包，生菜，肉
	{
		if (FoodVarity[0] >= 1 && FoodVarity[1] >= 1 && FoodVarity[3] >= 1)
		{
			for (int i = 0; i < 2; i++)
				FoodVarity[i] -= 1;
			FoodVarity[3] -= 1;
			//MessageBox(NULL, L"输出菜品", L"YES", MB_OK);
			PlaySound(L"music/FinishCooking.wav", NULL, SND_FILENAME | SND_ASYNC);
			IsGetScore = true;
			srand((int)time(0));
			menunum = rand() % 999;
			return true;
		}
	}
	if (MenuKind == 2) //面包，番茄，肉
	{
		if (FoodVarity[0] >= 1 && FoodVarity[2] >= 1 && FoodVarity[3] >= 1)
		{
			for (int i = 2; i < 4; i++)
				FoodVarity[i] -= 1;
			FoodVarity[0] -= 1;
			//MessageBox(NULL, L"输出菜品", L"YES", MB_OK);
			PlaySound(L"music/FinishCooking.wav", NULL, SND_FILENAME | SND_ASYNC);
			IsGetScore = true;
			srand((int)time(0));
			menunum = rand() % 999;
			return true;
		}
	}

	return false;
}

bool CanThrowFood(float x, float z)
{
	if (IsTomato[0] == 1 || IsLettuce[0] == 1 || IsBread[0] == 1 || IsMeatWell[0] == 1 || IsMeat[0] == 1 || IsHaveOverCookedMeat == 1)
	{
		if (z <= -2.8)
			if (x >= -7.0 && x <= -6.5)
			{
				PlaySound(L"music/ThrowFood.wav", NULL, SND_FILENAME | SND_ASYNC);
				IsBread[0] = 0;
				IsMeat[0] = 0;
				IsLettuce[0] = 0;
				IsTomato[0] = 0;
				IsMeatWell[0] = 0;
				IsHaveOverCookedMeat = 0;
				return true;
			}
	}
	return false;
}

void createmenu(int i) {
	if (i % 3 == 0) {
		//面包，生菜，番茄

		effect->GetVariableByName("Texture")->AsShaderResource()->SetResource(textureCuttingboard);
		technique->GetPassByIndex(0)->Apply(0, immediateContext);
		immediateContext->Draw(6, 318);
		effect->GetVariableByName("Texture")->AsShaderResource()->SetResource(texturePlatebread);
		technique->GetPassByIndex(0)->Apply(0, immediateContext);
		immediateContext->Draw(6, 324);
		effect->GetVariableByName("Texture")->AsShaderResource()->SetResource(texturePlatevegetable);
		technique->GetPassByIndex(0)->Apply(0, immediateContext);
		immediateContext->Draw(6, 330);
		effect->GetVariableByName("Texture")->AsShaderResource()->SetResource(texturePlatetomato);
		technique->GetPassByIndex(0)->Apply(0, immediateContext);
		immediateContext->Draw(6, 336);
		MenuKind = 0;
		Menu[0] = 1;
		Menu[1] = 1;
		Menu[2] = 1;
		Menu[3] = 0;
	}
	else if (i % 3 == 1) {
		//面包，生菜，肉

		effect->GetVariableByName("Texture")->AsShaderResource()->SetResource(textureCuttingboard);
		technique->GetPassByIndex(0)->Apply(0, immediateContext);
		immediateContext->Draw(6, 318);
		effect->GetVariableByName("Texture")->AsShaderResource()->SetResource(texturePlatebread);
		technique->GetPassByIndex(0)->Apply(0, immediateContext);
		immediateContext->Draw(6, 324);
		effect->GetVariableByName("Texture")->AsShaderResource()->SetResource(texturePlatevegetable);
		technique->GetPassByIndex(0)->Apply(0, immediateContext);
		immediateContext->Draw(6, 330);
		effect->GetVariableByName("Texture")->AsShaderResource()->SetResource(texturePlatemeat);
		technique->GetPassByIndex(0)->Apply(0, immediateContext);
		immediateContext->Draw(6, 336);
		MenuKind = 1;
		Menu[0] = 1;
		Menu[1] = 1;
		Menu[2] = 0;
		Menu[3] = 1;
	}
	else if (i % 3 == 2) {
		//面包，番茄，肉

		effect->GetVariableByName("Texture")->AsShaderResource()->SetResource(textureCuttingboard);
		technique->GetPassByIndex(0)->Apply(0, immediateContext);
		immediateContext->Draw(6, 318);
		effect->GetVariableByName("Texture")->AsShaderResource()->SetResource(texturePlatebread);
		technique->GetPassByIndex(0)->Apply(0, immediateContext);
		immediateContext->Draw(6, 324);
		effect->GetVariableByName("Texture")->AsShaderResource()->SetResource(texturePlatemeat);
		technique->GetPassByIndex(0)->Apply(0, immediateContext);
		immediateContext->Draw(6, 330);
		effect->GetVariableByName("Texture")->AsShaderResource()->SetResource(texturePlatetomato);
		technique->GetPassByIndex(0)->Apply(0, immediateContext);
		immediateContext->Draw(6, 336);
		MenuKind = 2;
		Menu[0] = 1;
		Menu[1] = 0;
		Menu[2] = 1;
		Menu[3] = 1;
	}
}

//
// 回调函数
//
LRESULT CALLBACK d3d::WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_DESTROY:
		::PostQuitMessage(0);
		break;

	case WM_KEYDOWN:
		if (wParam == 'F') {   //回车键重置游戏倒计时
			menuFlag = 1;
			ViewTurn = 1;
			GameStart = true;
			mytimer.Reset();
		}
		if (wParam == VK_ESCAPE)
			::DestroyWindow(hwnd);
		if (wParam == VK_F1)  //按F1键将光源类型改为方向光
			lightType = 0;
		if (wParam == VK_F2)  //按F2键将光源类型改为点光源
			lightType = 1;
		if (wParam == VK_F3)  //按F3键将光源类型改为聚光灯光源
			lightType = 2;
		break;
	}
	return ::DefWindowProc(hwnd, msg, wParam, lParam);
}

//
// 主函数WinMain
//
int WINAPI WinMain(HINSTANCE hinstance, HINSTANCE prevInstance, PSTR cmdLine, int showCmd)
{

	//初始化
	//**注意**:最上面声明的IDirect3DDevice9指针，在这里作为参数传给InitD3D函数
	if (!d3d::InitD3D(hinstance,
		800,
		600,
		&renderTargetView,
		&immediateContext,
		&swapChain,
		&device,
		&depthStencilBuffer,
		&depthStencilView))// [out]The created device.
	{
		::MessageBox(0, L"InitD3D() - FAILED", 0, 0);
		return 0;
	}

	if (!Setup())
	{
		::MessageBox(0, L"Setup() - FAILED", 0, 0);
		return 0;
	}

	d3d::EnterMsgLoop(Display);

	Cleanup();

	return 0;
}

//光源的常量缓存设置到Effect框架中
//由于光照设置比较复杂，所以以一个函数来进行设置
void SetLightEffect(Light light)
{
	//首先将光照类型，环境光强度，漫射光强度，镜面光强度设置到Effect中
	effect->GetVariableByName("type")->AsScalar()->SetInt(light.type);
	effect->GetVariableByName("LightAmbient")->AsVector()->SetFloatVector((float*)&(light.ambient));
	effect->GetVariableByName("LightDiffuse")->AsVector()->SetFloatVector((float*)&(light.diffuse));
	effect->GetVariableByName("LightSpecular")->AsVector()->SetFloatVector((float*)&(light.specular));

	//下面根据光照类型的不同设置不同的属性
	if (light.type == 0)  //方向光
	{
		//方向光只需要“方向”这个属性即可
		effect->GetVariableByName("LightDirection")->AsVector()->SetFloatVector((float*)&(light.direction));
		//将方向光的Tectnique设置到Effect
		technique = effect->GetTechniqueByName("T_DirLight");
	}
	else if (light.type == 1)  //点光源
	{
		//点光源需要“位置”，“常量衰变因子”，“一次衰变因子”，“二次衰变因子”
		effect->GetVariableByName("LightPosition")->AsVector()->SetFloatVector((float*)&(light.position));
		effect->GetVariableByName("LightAtt0")->AsScalar()->SetFloat(light.attenuation0);
		effect->GetVariableByName("LightAtt1")->AsScalar()->SetFloat(light.attenuation1);
		effect->GetVariableByName("LightAtt2")->AsScalar()->SetFloat(light.attenuation2);

		//将点光源的Tectnique设置到Effect
		technique = effect->GetTechniqueByName("T_PointLight");
	}
	else if (light.type == 2) //聚光灯光源
	{
		//点光源需要“方向”，“方向”，“常量衰变因子”，“一次衰变因子”，“二次衰变因子”
		//“内锥角度”，“外锥角度”，“聚光灯衰减系数”
		effect->GetVariableByName("LightPosition")->AsVector()->SetFloatVector((float*)&(light.position));
		effect->GetVariableByName("LightDirection")->AsVector()->SetFloatVector((float*)&(light.direction));

		effect->GetVariableByName("LightAtt0")->AsScalar()->SetFloat(light.attenuation0);
		effect->GetVariableByName("LightAtt1")->AsScalar()->SetFloat(light.attenuation1);
		effect->GetVariableByName("LightAtt2")->AsScalar()->SetFloat(light.attenuation2);

		effect->GetVariableByName("LightAlpha")->AsScalar()->SetFloat(light.alpha);
		effect->GetVariableByName("LightBeta")->AsScalar()->SetFloat(light.beta);
		effect->GetVariableByName("LightFallOff")->AsScalar()->SetFloat(light.fallOff);

		//将聚光灯光源的Tectnique设置到Effect
		technique = effect->GetTechniqueByName("T_SpotLight");
	}
}

