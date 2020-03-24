#include "d3dUtility.h"
#include "Light.h"
#include "gameTimer.h"
#include<mmsystem.h>  //���ڲ�����Ч��ͷ�ļ�


int ViewTurn = 0; //����ת���ӽ�
int menuFlag = 0; //����ʼ�˵�
int flag = 0;  //��������ʹ�
int DrawTime = 0;//��ֹ��δ������㻺��ռ�ÿռ�
int MenuKind = 0;//ָʾ��������Ҫ�����һ����Ʒ 
int IsOverCooked = 0;
int menunum = rand() % 100;
//Ԥ�������㻺��
D3D11_BUFFER_DESC bd;
HRESULT hr;
UINT vertexCount;
D3D11_SUBRESOURCE_DATA InitData;
ID3D11Buffer* vertexBuffer;
//����ȫ�ֵ�ָ��
ID3D11Device* device = NULL;//D3D11�豸�ӿ�
IDXGISwapChain* swapChain = NULL;//�������ӿ�
ID3D11DeviceContext* immediateContext = NULL;
ID3D11RenderTargetView* renderTargetView = NULL;//��ȾĿ����ͼ  

//Effect���ȫ��ָ��
ID3D11InputLayout* vertexLayout;
ID3DX11Effect* effect;
ID3DX11EffectTechnique* technique;

//������������ϵ����
XMMATRIX world;         //��������任�ľ���
XMMATRIX view;          //���ڹ۲�任�ľ���
XMMATRIX projection;    //����ͶӰ�任�ľ���

//�������ʺ͹��յ�ȫ�ֶ���
Material		boxMaterial;      //���Ӳ���
Material		floorMaterial;    //�ذ����
Material		waterMaterial;    //ˮ�����
Light			light[3];      //��Դ����
int             lightType = 0;  //��Դ����

ID3D11Texture2D* depthStencilBuffer;
ID3D11DepthStencilView* depthStencilView;  //���ģ����ͼ

ID3D11ShaderResourceView* textureBox;      //��������
ID3D11ShaderResourceView* textureFloor;    //�ذ�����
ID3D11ShaderResourceView* textureDrawer;    //��������
ID3D11ShaderResourceView* textureWall;    //ǽ������
ID3D11ShaderResourceView* textureWallTop;    //ǽ�涥������
ID3D11ShaderResourceView* textureScrollLine;    //���ʹ�����
ID3D11ShaderResourceView* texturePlayer1;    //��������
ID3D11ShaderResourceView* texturePlayer2;    //����2����
ID3D11ShaderResourceView* textureVegetable;  //�߲�����
ID3D11ShaderResourceView* texturePlateOfPalyer1;
ID3D11ShaderResourceView* textureBreadCutted;	//�кõ����
ID3D11ShaderResourceView* textureMeatCooked;
ID3D11ShaderResourceView* textureLettuceCutted;
ID3D11ShaderResourceView* textureTomatoCutted;	//�кõ�������

/*============================CC ========*/
ID3D11ShaderResourceView* texturePlatebread;      //�������
ID3D11ShaderResourceView* texturePlatemeat;
ID3D11ShaderResourceView* texturePlatevegetable;
ID3D11ShaderResourceView* texturePlatetomato;

//������ͷ����ʾ��ͼƬ
ID3D11ShaderResourceView* textureHeadbread;      //�������
ID3D11ShaderResourceView* textureHeadmeat;
ID3D11ShaderResourceView* textureHeadvegetable;
ID3D11ShaderResourceView* textureHeadtomato;

ID3D11ShaderResourceView* textureRubbishl;   //������
ID3D11ShaderResourceView* textureRubbishr;
ID3D11ShaderResourceView* texturePot;         //��
ID3D11ShaderResourceView* textureCuttingboard;  //�˰�
ID3D11ShaderResourceView* textureOut;     //����
//����Ϊ���ּ���״̬����ͼ
ID3D11ShaderResourceView* textureCooking;     //���ڼ���
ID3D11ShaderResourceView* textureCookedWell;     //������
ID3D11ShaderResourceView* textureOverCooked;     //������
ID3D11ShaderResourceView* textureOverCookedMeat;     //���˵���
/*============================CC ========*/

//ʱ����������
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

//��Ϸ��ʼ��������
ID3D11ShaderResourceView* startGame;

ID3D11BlendState* blendStateAlpha;        //���״̬
ID3D11RasterizerState* noCullRS;          //��������״̬


//ָʾ����ʳ��Ļ�ȡ״̬,��ά����ֱ��ʾ������
int IsTomato[2] = { 0,0 };
int IsBread[2] = { 0,0 };
int IsMeat[2] = { 0,0 };
int IsLettuce[2] = { 0,0 };
//ָʾ����׼���õ�ʳ��Ļ�ȡ״̬����ά����ֱ��ʾ������
int IsTomatoWell[2] = { 0,0 };
int IsBreadWell[2] = { 0,0 };
int IsMeatWell[2] = { 0,0 };
int IsLettuceWell[2] = { 0,0 };
//ָʾ���������ϴ��ʹ���ʳ��״̬
int IsTomatoInSC = 0;
int IsBreadInSC = 0;
int IsMeatWellInSC = 0;
int IsLettuceInSC = 0;
int Cooking = 0;//ָʾ�Ƿ��ڼ���
int Cutting = 0;//ָʾ�Ƿ����в�
int IsCuttedWell = 0;//ָʾ�Ƿ��к��˲�
int IsMeatCookedWell = 0;//ָʾ���Ƿ���
int IsHaveOverCookedMeat = 0;//ָʾ����ͷ���Ƿ��м���˵���
int FoodVarity[4] = { 0,0,0,0 };//ָʾ������ڵ��ѷ���ʳ������ֱ��ʾ��������ˣ����Ѻ���
float PutToSCXMove = 0.0f;//ָʾʳ��Ӧ���ڴ��ʹ��ϵ���ʼλ��
int Menu[4] = { 0,0,0,0 };  //�˵�


ID3D11ShaderResourceView* texturePlayer1GetFood = texturePlateOfPalyer1;  // ���1ȡ��ʳ�������
ID3D11ShaderResourceView* texturePlayer2GetFood;
static int getFoodPlayer1VertexNums = 0;      //���1ȡ��ʳ��Ķ�����
static int getFoodPlayer2VertexNums = 0;

void SetLightEffect(Light light);
void putFoodInScroll(int playerNum, float startPos, float stepLen, float timeDelta, int vertexNums, ID3D11ShaderResourceView* textureTemp);
//�ı�����ͷ�����ӵ�����
void setTextureOfPlayerHead(int vertexNums, float tmpMoveX, float tmpMoveZ, ID3D11ShaderResourceView* textureTempChange);

/*============================CC ========*/
bool CanGetFood(float x, float z);//�жϿɲ������õ���
bool CanCookMeat(float x, float z);//�ж��ǲ��ǿ������
bool CanCutVegtable(float x, float z);//�ж��ǲ��ǿ����в�
bool CanPutInScroll(float x, float z);//�ж��ǲ��ǿ��Խ�ʳ����ϴ��ʹ�
bool CanGetFromScroll(float x, float z);//�ж��ǲ��ǿ��ԴӴ��ʹ���ȡʳ��
bool SendOutFood(float x, float z);//�ж�ʳ���Ƿ񱻷���������� �ֱ��� ��� ���� ���� ��
bool CanThrowFood(float x, float z);//�ж���������ǲ��ǿ���������Ͱ��ʳ��
/*============================CC ========*/
void createmenu(int i);      //���ɲ˵�
static int RewardScore = 8;           //����һ���ˣ���ҵõ�8��
GameTimer mytimer = GameTimer();     //��Ϸ��ʱ��
GameTimer cookingTimer = GameTimer();     //���ڼ�ʱ���⻨�˶��
static bool IsGetScore = false;     //�ж�������ܲ��ܵ÷֣�false��ʾ����˵�Ҫ�󲻷��ϣ�true��ʾ����Ҫ�󣬿��Ե÷�
static bool GameStart = false;          //��Ϸ��ʼ��־
static bool GameOver = false;          //��Ϸ������־


//����һ������ṹ����������������ͷ���������������
struct Vertex
{
	XMFLOAT3 Pos;   //����
	XMFLOAT3 Normal;	//���շ�����
	XMFLOAT2 Tex;	//����
};

//������Ҫ�������ӣ����ӣ��Լ�ˮ��Ķ���
Vertex vertices[] =
{
	//�²�߽�
	{ XMFLOAT3(-7.0f,0.3f,-4.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
	{ XMFLOAT3(8.0f, 0.3f, -4.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(15.0f, 0.0f) },
	{ XMFLOAT3(-7.0f, 0.3f, -5.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(0.0f, 1.0f) },

	{ XMFLOAT3(8.0f, 0.3f, -4.0f),XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(15.0f, 0.0f) },
	{ XMFLOAT3(8.0f,0.3f,-5.0f),XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(15.0f, 1.0f) },
	{ XMFLOAT3(-7.0f, 0.3f, -5.0f),XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(0.0f, 1.0f) },
	//�Ҳ�߽�
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
	//�ϲ�߽�
	{ XMFLOAT3(-7.0f,0.3f,5.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(0.0f, 1.0f) },
	{ XMFLOAT3(-7.0f, 0.3f, 6.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
	{ XMFLOAT3(8.0f, 0.3f, 5.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(15.0f, 1.0f) },

	{ XMFLOAT3(-7.0f,0.3f,6.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
	{ XMFLOAT3(8.0f, 0.3f, 6.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(15.0f, 0.0f) },
	{ XMFLOAT3(8.0f, 0.3f, 5.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(15.0f, 1.0f) },
	//���߽�
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
	//���ʹ�
	{ XMFLOAT3(-7.0f,0.3f,1.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
	{ XMFLOAT3(8.0f, 0.3f, 0.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(15.0f, 1.0f) },
	{ XMFLOAT3(-7.0f, 0.3f, 0.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(0.0f, 1.0f) },

	{ XMFLOAT3(-7.0f,0.3f,1.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
	{ XMFLOAT3(8.0f, 0.3f, 1.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(15.0f, 0.0f) },
	{ XMFLOAT3(8.0f, 0.3f, 0.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(15.0f, 1.0f) },

	//������ǽ�������
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
	//���·�������ͼ 42
	{ XMFLOAT3(7.0f,0.3f,-1.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(1.0f, 0.0f) },
	{ XMFLOAT3(7.0f,0.3f,-4.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(1.0f, 3.0f) },
	{ XMFLOAT3(7.0f,0.0f,-1.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },

	{ XMFLOAT3(7.0f,0.0f,-1.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
	{ XMFLOAT3(7.0f,0.3f,-4.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(1.0f, 3.0f) },
	{ XMFLOAT3(7.0f,0.0f,-4.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(0.0f, 3.0f) },

	//�Ҳ��в��Ϸ����� 48
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

	//�Ҳ��в��Ϸ�
	{ XMFLOAT3(4.0f,0.3f,0.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(0.0f, 1.0f) },
	{ XMFLOAT3(7.0f,0.3f,0.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(3.0f, 1.0f) },
	{ XMFLOAT3(7.0f,0.3f,-1.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(3.0f, 0.0f) },

	{ XMFLOAT3(4.0f,0.3f,0.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(0.0f, 1.0f) },
	{ XMFLOAT3(7.0f,0.3f,-1.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(3.0f, 0.0f) },
	{ XMFLOAT3(4.0f,0.3f,-1.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },

	//������
	{ XMFLOAT3(-6.0f,0.3f,0.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(1.0f, 0.0f) },
	{ XMFLOAT3(-6.0f,0.0f,0.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
	{ XMFLOAT3(-6.0f,0.0f,-4.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(0.0f, 3.0f) },

	{ XMFLOAT3(-6.0f,0.3f,0.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(1.0f, 0.0f) },
	{ XMFLOAT3(-6.0f,0.0f,-4.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(0.0f, 3.0f) },
	{ XMFLOAT3(-6.0f,0.3f,-4.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(1.0f, 3.0f) },

	//���ϴ��ʹ�����
	{ XMFLOAT3(-6.0f,0.3f,0.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
	{ XMFLOAT3(-3.0f,0.3f,0.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(3.0f, 0.0f) },
	{ XMFLOAT3(-3.0f,0.0f,0.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(3.0f, 1.0f) },

	{ XMFLOAT3(-6.0f,0.3f,0.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
	{ XMFLOAT3(-3.0f,0.0f,0.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(3.0f, 1.0f) },
	{ XMFLOAT3(-6.0f,0.0f,0.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(0.0f, 1.0f) },

	//�Ҵ��ʹ�����
	{ XMFLOAT3(0.0f,0.3f,0.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
	{ XMFLOAT3(4.0f,0.3f,0.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(4.0f, 0.0f) },
	{ XMFLOAT3(4.0f,0.0f,0.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(4.0f, 1.0f) },

	{ XMFLOAT3(0.0f,0.3f,0.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
	{ XMFLOAT3(4.0f,0.0f,0.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(4.0f, 1.0f) },
	{ XMFLOAT3(0.0f,0.0f,0.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(0.0f, 1.0f) },

	//���²�������ǽ�ڲ���
	{ XMFLOAT3(-3.0f,0.3f,-1.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
	{ XMFLOAT3(0.0f,0.3f,-1.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(3.0f, 0.0f) },
	{ XMFLOAT3(0.0f,0.0f,-1.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(3.0f, 1.0f) },

	{ XMFLOAT3(-3.0f,0.3f,-1.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
	{ XMFLOAT3(0.0f,0.0f,-1.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(3.0f, 1.0f) },
	{ XMFLOAT3(-3.0f,0.0f,-1.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(0.0f, 1.0f) },
	//���������ߣ�������
	{ XMFLOAT3(-3.0f,0.3f,2.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(1.0f, 0.0f) },
	{ XMFLOAT3(-3.0f,0.0f,2.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
	{ XMFLOAT3(-3.0f,0.0f,1.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(0.0f, 1.0f) },

	{ XMFLOAT3(-3.0f,0.3f,2.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(0.0f, 1.0f) },
	{ XMFLOAT3(-3.0f,0.0f,1.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(1.0f, 0.0f) },
	{ XMFLOAT3(-3.0f,0.3f,1.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) },
	//���������ߣ�������
	{ XMFLOAT3(0.0f,0.3f,0.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(1.0f, 0.0f) },
	{ XMFLOAT3(0.0f,0.0f,0.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
	{ XMFLOAT3(0.0f,0.0f,-1.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(0.0f, 1.0f) },

	{ XMFLOAT3(0.0f,0.3f,0.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(0.0f, 1.0f) },
	{ XMFLOAT3(0.0f,0.0f,-1.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(1.0f, 0.0f) },
	{ XMFLOAT3(0.0f,0.3f,-1.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) },

	//����ǽ�ڲ���
	{ XMFLOAT3(-6.0f,0.3f,5.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(1.0f, 0.0f) },
	{ XMFLOAT3(-6.0f,0.0f,5.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
	{ XMFLOAT3(-6.0f,0.0f,2.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(0.0f, 3.0f) },

	{ XMFLOAT3(-6.0f,0.3f,5.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(1.0f, 0.0f) },
	{ XMFLOAT3(-6.0f, 0.0f, 2.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(0.0f, 3.0f) },
	{ XMFLOAT3(-6.0f,0.3f,2.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(1.0f, 3.0f) },

	//�ϱ�Եǽ�ڲ���
	{ XMFLOAT3(-6.0f,0.3f,5.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
	{ XMFLOAT3(7.0f,0.3f,5.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(13.0f, 0.0f) },
	{ XMFLOAT3(7.0f,0.0f,5.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(13.0f, 1.0f) },

	{ XMFLOAT3(-6.0f,0.3f,5.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
	{ XMFLOAT3(7.0f,0.0f,5.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(13.0f, 1.0f) },
	{ XMFLOAT3(-6.0f,0.0f,5.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(0.0f, 1.0f) },

	//����������ڲ��
	{ XMFLOAT3(7.0f,0.3f,5.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(1.0f, 0.0f) },
	{ XMFLOAT3(7.0f,0.3f,3.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(1.0f, 2.0f) },
	{ XMFLOAT3(7.0f,0.0f,3.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(0.0f, 2.0f) },

	{ XMFLOAT3(7.0f,0.3f,5.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(1.0f, 0.0f) },
	{ XMFLOAT3(7.0f,0.0f,3.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(0.0f, 2.0f) },
	{ XMFLOAT3(7.0f,0.0f,5.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },


	//�����������ڱ�
	{ XMFLOAT3(7.0f,0.3f,3.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(1.0f, 0.0f) },
	{ XMFLOAT3(7.0f,0.3f,1.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(1.0f, 2.0f) },
	{ XMFLOAT3(7.0f,0.0f,1.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(0.0f, 2.0f) },

	{ XMFLOAT3(7.0f,0.3f,3.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(1.0f, 0.0f) },
	{ XMFLOAT3(7.0f,0.0f,1.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(0.0f, 2.0f) },
	{ XMFLOAT3(7.0f,0.0f,3.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
	//22*3 28*3
	//�ذ�
	{ XMFLOAT3(10.0f,0.0f,10.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
	{ XMFLOAT3(10.0f,0.0f,-10.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(10.0f, 0.0f) },
	{ XMFLOAT3(-10.0f,0.0f,10.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(0.0f, 10.0f) },

	{ XMFLOAT3(-10.0f,0.0f,10.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(0.0f, 10.0f) },
	{ XMFLOAT3(10.0f,0.0f,-10.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(10.0f, 0.0f) },
	{ XMFLOAT3(-10.0f,0.0f,-10.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(10.0f,10.0f) },

	//������������Χ�Ķ�ǽ

	//���ǽ 156
	{ XMFLOAT3(-7.0f,2.5f,6.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(4.0f, 0.0f) },
	{ XMFLOAT3(-7.0f,0.0f,6.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(4.0f, 2.5f) },
	{ XMFLOAT3(-7.0f,0.0f,-5.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(0.0f, 2.5f) },

	{ XMFLOAT3(-7.0f,2.5f,6.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(4.0f, 0.0f) },
	{ XMFLOAT3(-7.0f,0.0f,-5.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(0.0f, 2.5f) },
	{ XMFLOAT3(-7.0f,2.5f,-5.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },

	//�±�ǽ 162
	{ XMFLOAT3(-7.0f,2.5f,-5.5f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
	{ XMFLOAT3(8.0f,2.5f,-5.5f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(11.0f, 0.0f) },
	{ XMFLOAT3(8.0f,0.0f,-5.5f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(11.0f, 2.5f) },

	{ XMFLOAT3(-7.0f,2.5f,-5.5f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
	{ XMFLOAT3(8.0f,0.0f,-5.5f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(11.0f, 2.5f) },
	{ XMFLOAT3(-7.0f,0.0f,-5.5f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(0.0f, 2.5f) },

	//�ϱ�ǽ 168
	{ XMFLOAT3(-7.0f,2.5f,6.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
	{ XMFLOAT3(8.0f,2.5f,6.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(6.0f, 0.0f) },
	{ XMFLOAT3(8.0f,0.0f,6.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(6.0f, 2.5f) },

	{ XMFLOAT3(-7.0f,2.5f,6.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
	{ XMFLOAT3(8.0f,0.0f,6.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(6.0f, 2.5f) },
	{ XMFLOAT3(-7.0f,0.0f,6.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(0.0f, 2.5f) },

	//�Ҳ�ǽ
	//�ϰ�� 174
	{ XMFLOAT3(8.0f,2.5f,6.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
	{ XMFLOAT3(8.0f,2.5f,5.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(0.5f, 0.0f) },
	{ XMFLOAT3(8.0f,0.0f,5.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(0.5f, 2.5f) },

	{ XMFLOAT3(8.0f,2.5f,6.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
	{ XMFLOAT3(8.0f,0.0f,5.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(0.5f, 2.5f) },
	{ XMFLOAT3(8.0f,0.0f,6.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(0.0f, 2.5f) },

	//�°�� 180
	{ XMFLOAT3(8.0f,2.5f,3.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
	{ XMFLOAT3(8.0f,2.5f,-5.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(3.0f, 0.0f) },
	{ XMFLOAT3(8.0f,0.0f,-5.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(3.0f, 2.5f) },

	{ XMFLOAT3(8.0f,2.5f,3.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
	{ XMFLOAT3(8.0f,0.0f,-5.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(3.0f, 2.5f) },
	{ XMFLOAT3(8.0f,0.0f,3.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(0.0f, 2.5f) },

	//����������ǽ�Ķ���
			//���ǽ���� 186
	{ XMFLOAT3(-7.5f,2.5f,6.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(4.0f, 0.0f) },
	{ XMFLOAT3(-7.0f,2.5f,6.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(4.0f, 1.0f) },
	{ XMFLOAT3(-7.0f,2.5f,-5.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(0.0f, 1.0f) },

	{ XMFLOAT3(-7.5f,2.5f,6.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(4.0f, 0.0f) },
	{ XMFLOAT3(-7.0f,2.5f,-5.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(0.0f, 1.0f) },
	{ XMFLOAT3(-7.5f,2.5f,-5.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },


	//�±�ǽ���� 192
	{ XMFLOAT3(-7.5f,2.5f,-5.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
	{ XMFLOAT3(8.0f,2.5f,-5.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(6.0f, 0.0f) },
	{ XMFLOAT3(8.0f,2.5f,-5.5f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(6.0f, 1.0f) },

	{ XMFLOAT3(-7.5f,2.5f,-5.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
	{ XMFLOAT3(8.0f,2.5f,-5.5f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(6.0f, 1.0f) },
	{ XMFLOAT3(-7.5f,2.5f,-5.5f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(0.0f, 1.0f) },

	//�ϱ�ǽ���� 198
	{ XMFLOAT3(-7.5f,2.5f,6.5), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
	{ XMFLOAT3(8.5f,2.5f,6.5), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(4.0f, 0.0f) },
	{ XMFLOAT3(8.5f,2.5,6.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(4.0f, 1.0f) },

	{ XMFLOAT3(-7.5f,2.5f,6.5), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
	{ XMFLOAT3(8.5f,2.5,6.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(4.0f, 1.0f) },
	{ XMFLOAT3(-7.5f,2.5,6.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(0.0f, 1.0f) },

	//�ұ�ǽ���� 204
	{ XMFLOAT3(8.0f,2.5f,6.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(4.0f, 0.0f) },
	{ XMFLOAT3(8.5f,2.5f,6.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(4.0f, 1.0f) },
	{ XMFLOAT3(8.5f,2.5,-5.5f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(0.0f, 1.0f) },

	{ XMFLOAT3(8.0f,2.5f,6.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(4.0f, 0.0f) },
	{ XMFLOAT3(8.5f,2.5f,-5.5f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(0.0f, 1.0f) },
	{ XMFLOAT3(8.0f,2.5f,-5.5f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },

	//�ұ�ǽ�ϰ벿�֣����� 210
	{ XMFLOAT3(8.0f,2.5f,5.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
	{ XMFLOAT3(8.5f,2.5f,5.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
	{ XMFLOAT3(8.5f,0.0,5.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },

	{ XMFLOAT3(8.0f,2.5f,5.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
	{ XMFLOAT3(8.5f,0.0f,5.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
	{ XMFLOAT3(8.0f,0.0f,5.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },


	//����Ķ���  216
	{ XMFLOAT3(2.0f, 1.0f, -0.7f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(0.0f, 0.25f) },
	{ XMFLOAT3(1.0f, 1.0f, -0.7f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(0.167f, 0.25f) },
	{ XMFLOAT3(2.0f, 0.0f, -1.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(0.0f, 0.50f) },

	{ XMFLOAT3(2.0f, 0.0f, -1.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(0.0f, 0.50f) },
	{ XMFLOAT3(1.0f, 1.0f, -0.7f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(0.167f, 0.25f) },
	{ XMFLOAT3(1.0f, 0.0f, -1.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(0.167f, 0.50f) },

	//����2�Ķ���  222
	{ XMFLOAT3(-2.0f, 1.0f, 2.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(0.0f, 0.25f) },
	{ XMFLOAT3(-1.0f, 1.0f, 2.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(0.167f, 0.25f) },
	{ XMFLOAT3(-2.0f, 0.0f, 2.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(0.0f, 0.50f) },

	{ XMFLOAT3(-2.0f, 0.0f, 2.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(0.0f, 0.50f) },
	{ XMFLOAT3(-1.0f, 1.0f, 2.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(0.167f, 0.25f) },
	{ XMFLOAT3(-1.0f, 0.0f, 2.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(0.167f, 0.50f) },

	//���ʹ�  228
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
//����  240
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
//������  264
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

//�� 276
{ XMFLOAT3(-6.0f,0.32f,-1.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(1.0f, 0.0f) },
{ XMFLOAT3(-6.0f, 0.32f, -2.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) },
{ XMFLOAT3(-7.0f, 0.32f, -1.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },

{ XMFLOAT3(-7.0f, 0.32f, -1.0f),XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
{ XMFLOAT3(-6.0f,0.32f,-2.0f),XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) },
{ XMFLOAT3(-7.0f, 0.32f, -2.0f),XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(0.0f, 1.0f) },

//�˰� 282
{ XMFLOAT3(-1.0f,0.32f,5.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(0.0f, 1.0f) },
{ XMFLOAT3(-1.0f, 0.32f, 6.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
{ XMFLOAT3(0.0f, 0.32f, 5.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) },

{ XMFLOAT3(-1.0f,0.32f,6.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
{ XMFLOAT3(0.0f, 0.32f, 6.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(1.0f, 0.0f) },
{ XMFLOAT3(0.0f, 0.32f, 5.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) },

//���� 288
{ XMFLOAT3(8.5f,0.31f,5.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(1.0f, 0.0f) },
{ XMFLOAT3(8.5f, 0.31f, 3.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) },
{ XMFLOAT3(7.0f, 0.31f, 5.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },

{ XMFLOAT3(7.0f, 0.31f, 5.0f),XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
{ XMFLOAT3(8.5f,0.31f,3.0f),XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) },
{ XMFLOAT3(7.0f, 0.31f, 3.0f),XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(0.0f, 1.0f) },
/*======================================cc =========*/


// ʳ��test  294
{ XMFLOAT3(5.0f, 0.35f, 1.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
{ XMFLOAT3(6.0f, 0.35f, 0.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) },
{ XMFLOAT3(5.0f, 0.35f, 0.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(0.0f, 1.0f) },

{ XMFLOAT3(5.0f, 0.35f, 1.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
{ XMFLOAT3(6.0f, 0.35f, 1.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(1.0f, 0.0f) },
{ XMFLOAT3(6.0f, 0.35f, 0.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) },

//����ͷ�������� 300
{ XMFLOAT3(1.7f, 1.4f, -0.2f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
{ XMFLOAT3(1.2f, 1.4f, -0.2f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(1.0f, 0.0f) },
{ XMFLOAT3(1.7f, 1.1f, -1.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(0.0f, 1.0f) },

{ XMFLOAT3(1.7f, 1.1f, -1.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(0.0f, 1.0f) },
{ XMFLOAT3(1.2f, 1.4f, -0.2f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(1.0f, 0.0f) },
{ XMFLOAT3(1.2f, 1.1f, -1.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) },

//����2ͷ�������� 306
{ XMFLOAT3(-1.7f, 1.4f, 2.5f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
{ XMFLOAT3(-1.2f, 1.4f, 2.5f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(1.0f, 0.0f) },
{ XMFLOAT3(-1.7f, 1.1f, 2.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(0.0f, 1.0f) },

{ XMFLOAT3(-1.7f, 1.1f, 2.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(0.0f, 1.0f) },
{ XMFLOAT3(-1.2f, 1.4f, 2.5f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(1.0f, 0.0f) },
{ XMFLOAT3(-1.2f, 1.1f, 2.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) },


//������  312
{ XMFLOAT3(-6.0f,0.31f,-4.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
{ XMFLOAT3(-5.0f, 0.31f, -5.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) },
{ XMFLOAT3(-6.0f, 0.31f, -5.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(0.0f, 1.0f) },

{ XMFLOAT3(-6.0f,0.31f,-4.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
{ XMFLOAT3(-5.0f, 0.31f, -4.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(1.0f, 0.0f) },
{ XMFLOAT3(-5.0f, 0.31f, -5.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) },

//�˵� 318
{ XMFLOAT3(-4.5f, 5.5f, 0.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },  //����
{ XMFLOAT3(-3.5f, 5.5f, 0.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(1.0f, 0.0f) },   //����
{ XMFLOAT3(-3.5f, 4.5f, -1.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) },   //����

{ XMFLOAT3(-4.5f, 5.5f, 0.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },//����
{ XMFLOAT3(-3.5f, 4.5f, -1.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) }, //����
{ XMFLOAT3(-4.5f, 4.5f, -1.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(0.0f, 1.0f) }, //����

{ XMFLOAT3(-4.5f, 4.5f, -1.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },  //����
{ XMFLOAT3(-4.17f, 4.5f, -1.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(1.0f, 0.0f) },   //����
{ XMFLOAT3(-4.17f, 4.17f, -1.3f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) },   //����

{ XMFLOAT3(-4.5f, 4.5f, -1.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },//����
{ XMFLOAT3(-4.17f, 4.17f, -1.3f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) }, //����
{ XMFLOAT3(-4.5f, 4.17f, -1.33f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(0.0f, 1.0f) }, //����

{ XMFLOAT3(-4.16f, 4.5f, -1.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },  //����
{ XMFLOAT3(-3.83f, 4.5f, -1.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(1.0f, 0.0f) },   //����
{ XMFLOAT3(-3.83f, 4.17f,-1.33f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) },   //����

{ XMFLOAT3(-4.16f, 4.5f, -1.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },//����
{ XMFLOAT3(-3.83f, 4.17f,-1.33f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) }, //����
{ XMFLOAT3(-4.16f, 4.17f, -1.33f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(0.0f, 1.0f) }, //����

{ XMFLOAT3(-3.82f, 4.5f, -1.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },  //����
{ XMFLOAT3(-3.5f, 4.5f, -1.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(1.0f, 0.0f) },   //����
{ XMFLOAT3(-3.5f, 4.17f, -1.33f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) },   //����

{ XMFLOAT3(-3.82f, 4.5f, -1.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },//����
{ XMFLOAT3(-3.5f, 4.17f, -1.33f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) }, //����
{ XMFLOAT3(-3.82f, 4.17f, -1.33f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(0.0f, 1.0f) }, //����

	//ʱ�����  342
{ XMFLOAT3(-0.55f, 5.5f, 0.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },//����
{ XMFLOAT3(-0.1f, 5.0f, 0.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) }, //����
{ XMFLOAT3(-0.35f, 5.0f, 0.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(0.0f, 1.0f) }, //����

{ XMFLOAT3(-0.55f, 5.5f, 0.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },  //����
{ XMFLOAT3(-0.3f, 5.5f, 0.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(1.0f, 0.0f) },   //����
{ XMFLOAT3(-0.1f, 5.0f, 0.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) },   //����

//�������  348
{ XMFLOAT3(-0.55f, 5.5f, 0.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },//����
{ XMFLOAT3(-0.3f, 5.0f, 0.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) }, //����
{ XMFLOAT3(-0.55f, 5.0f, 0.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(0.0f, 1.0f) }, //����

{ XMFLOAT3(-0.55f, 5.5f, 0.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },  //����
{ XMFLOAT3(-0.3f, 5.5f, 0.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(1.0f, 0.0f) },   //����
{ XMFLOAT3(-0.3f, 5.0f, 0.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) },   //����

	//������ҿ�ʼ��Ϸ���  354
{ XMFLOAT3(-2.5f, 5.5f, -0.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
{ XMFLOAT3(1.0f, 1.0f, -0.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) },
{ XMFLOAT3(-2.5f, 1.0f, -0.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(0.0f, 1.0f) },

{ XMFLOAT3(-2.5f, 5.5f, -0.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
{ XMFLOAT3(1.0f, 5.5f, -0.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(1.0f, 0.0f) },
{ XMFLOAT3(1.0f, 1.0f, -0.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) },

};

//**************����Ϊ��ܺ���******************
bool Setup()
{
	//������Ҫ����5����Ҫ����
	//��һ�������ⲿ�ļ�������fx�ļ���ͼ���ļ���
	//�ڶ�������������Ⱦ״̬
	//�������������벼��
	//���Ĳ��������㻺��
	//���岽���ò��ʺ͹���
	//*************��һ�������ⲿ�ļ�������fx�ļ���ͼ���ļ���****************************
	hr = S_OK;              //����HRESULT�Ķ������ڼ�¼���������Ƿ�ɹ�
	ID3DBlob* pTechBlob = NULL;     //����ID3DBlob�Ķ������ڴ�Ŵ��ļ���ȡ����Ϣ
	//������֮ǰ������.fx�ļ���ȡ��ɫ�������Ϣ
	hr = D3DX11CompileFromFile(L"Shader.fx", NULL, NULL, NULL, "fx_5_0",
		D3DCOMPILE_ENABLE_STRICTNESS, 0, NULL, &pTechBlob, NULL, NULL);
	if (FAILED(hr))
	{
		::MessageBox(NULL, L"fx�ļ�����ʧ��", L"Error", MB_OK); //�����ȡʧ�ܣ�����������Ϣ
		return hr;
	}
	//����D3DX11CreateEffectFromMemory����ID3DEffect����
	hr = D3DX11CreateEffectFromMemory(pTechBlob->GetBufferPointer(),
		pTechBlob->GetBufferSize(), 0, device, &effect);

	if (FAILED(hr))
	{
		::MessageBox(NULL, L"����Effectʧ��", L"Error", MB_OK);  //����ʧ�ܣ�����������Ϣ
		return hr;
	}
	//���ⲿͼ���ļ���������
	{
		////��������
		D3DX11CreateShaderResourceViewFromFile(device, L"img/desktop.jpg", NULL, NULL, &textureBox, NULL);
		////�ذ������
		D3DX11CreateShaderResourceViewFromFile(device, L"img/floor.jpg", NULL, NULL, &textureFloor, NULL);
		////��������
		D3DX11CreateShaderResourceViewFromFile(device, L"img/drawer.jpg", NULL, NULL, &textureDrawer, NULL);
		//ǽ����
		D3DX11CreateShaderResourceViewFromFile(device, L"img/Wall.PNG", NULL, NULL, &textureWall, NULL);
		//ǽ����
		D3DX11CreateShaderResourceViewFromFile(device, L"img/WallTop.PNG", NULL, NULL, &textureWallTop, NULL);
		//���ʹ�����
		D3DX11CreateShaderResourceViewFromFile(device, L"img/ScrollLine.PNG", NULL, NULL, &textureScrollLine, NULL);
		//��������
		D3DX11CreateShaderResourceViewFromFile(device, L"img/Role1.png", NULL, NULL, &texturePlayer1, NULL);
		//����2����
		D3DX11CreateShaderResourceViewFromFile(device, L"img/Role2.png", NULL, NULL, &texturePlayer2, NULL);
		//testʳ������
		D3DX11CreateShaderResourceViewFromFile(device, L"food/plate-vegetable.png", NULL, NULL, &textureVegetable, NULL);
		//����1ͷ����������
		D3DX11CreateShaderResourceViewFromFile(device, L"food/plate.png", NULL, NULL, &texturePlateOfPalyer1, NULL);

		//������⡢���ѡ��߲ˡ�����
		D3DX11CreateShaderResourceViewFromFile(device, L"food/plate-bread.png", NULL, NULL, &texturePlatebread, NULL);
		D3DX11CreateShaderResourceViewFromFile(device, L"food/plate-meat.png", NULL, NULL, &texturePlatemeat, NULL);
		D3DX11CreateShaderResourceViewFromFile(device, L"food/plate-tomato.png", NULL, NULL, &texturePlatetomato, NULL);
		D3DX11CreateShaderResourceViewFromFile(device, L"food/plate-vegetable.png", NULL, NULL, &texturePlatevegetable, NULL);

		//����ͷ���ϵ�������⡢���ѡ��߲ˡ�����
		D3DX11CreateShaderResourceViewFromFile(device, L"food/head-bread.png", NULL, NULL, &textureHeadbread, NULL);
		D3DX11CreateShaderResourceViewFromFile(device, L"food/head-meat.png", NULL, NULL, &textureHeadmeat, NULL);
		D3DX11CreateShaderResourceViewFromFile(device, L"food/head-tomato.png", NULL, NULL, &textureHeadtomato, NULL);
		D3DX11CreateShaderResourceViewFromFile(device, L"food/head-vegetable.png", NULL, NULL, &textureHeadvegetable, NULL);
		//����ͷ���ϵļ���˵���
		D3DX11CreateShaderResourceViewFromFile(device, L"food/head-meat-overcooked.png", NULL, NULL, &textureOverCookedMeat, NULL);

		//������
		D3DX11CreateShaderResourceViewFromFile(device, L"food/Rubbishl.png", NULL, NULL, &textureRubbishl, NULL);
		D3DX11CreateShaderResourceViewFromFile(device, L"food/Rubbishr.png", NULL, NULL, &textureRubbishr, NULL);
		//��
		D3DX11CreateShaderResourceViewFromFile(device, L"food/pot.png", NULL, NULL, &texturePot, NULL);
		//�˰�
		D3DX11CreateShaderResourceViewFromFile(device, L"food/Cuttingboard.png", NULL, NULL, &textureCuttingboard, NULL);
		//����
		D3DX11CreateShaderResourceViewFromFile(device, L"img/out.PNG", NULL, NULL, &textureOut, NULL);
		//�кõ�ʳ������
		D3DX11CreateShaderResourceViewFromFile(device, L"food/bread-cutted.png", NULL, NULL, &textureBreadCutted, NULL);
		D3DX11CreateShaderResourceViewFromFile(device, L"food/meat-cooked.png", NULL, NULL, &textureMeatCooked, NULL);
		D3DX11CreateShaderResourceViewFromFile(device, L"food/lettuce-cutted.png", NULL, NULL, &textureLettuceCutted, NULL);
		D3DX11CreateShaderResourceViewFromFile(device, L"food/tomato-cutted.png", NULL, NULL, &textureTomatoCutted, NULL);

		//����ʱ����Ҫ�õ�������
		D3DX11CreateShaderResourceViewFromFile(device, L"food/cooking.png", NULL, NULL, &textureCooking, NULL);
		D3DX11CreateShaderResourceViewFromFile(device, L"food/cooked.png", NULL, NULL, &textureCookedWell, NULL);
		D3DX11CreateShaderResourceViewFromFile(device, L"food/overcooked.png", NULL, NULL, &textureOverCooked, NULL);

		//��������
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
		//���������Ϸ״̬������
		D3DX11CreateShaderResourceViewFromFile(device, L"img/StartGame.png", NULL, NULL, &startGame, NULL);
	}
	//�����ʼ������


	//*************��һ�������ⲿ�ļ�������fx�ļ���ͼ���ļ���****************************

	//*************�ڶ�������������Ⱦ״̬************************************************
	//�ȴ���һ�����״̬������
	D3D11_BLEND_DESC blendDesc;
	ZeroMemory(&blendDesc, sizeof(blendDesc)); //�������
	blendDesc.AlphaToCoverageEnable = false;     //�ر�AlphaToCoverage���ز�������
	blendDesc.IndependentBlendEnable = false;    //����Զ��RenderTargetʹ�ò�ͬ�Ļ��״̬
	//ֻ���RenderTarget[0]���û��ƻ��״̬������1-7
	blendDesc.RenderTarget[0].BlendEnable = true;                   //�������
	blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;     //����Դ����
	blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;//����Ŀ������
	blendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;         //��ϲ���
	blendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;      //Դ��ϰٷֱ�����
	blendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;    //Ŀ���ϰٷֱ�����
	blendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;    //��ϰٷֱȵĲ���
	blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;  //д����
	//����ID3D11BlendState�ӿ�
	device->CreateBlendState(&blendDesc, &blendStateAlpha);

	//�رձ�������
	D3D11_RASTERIZER_DESC ncDesc;
	ZeroMemory(&ncDesc, sizeof(ncDesc));  //�������
	ncDesc.CullMode = D3D11_CULL_NONE;   //�޳��ض�����������Σ����ﲻ�޳�����ȫ������
	ncDesc.FillMode = D3D11_FILL_SOLID;  //���ģʽ������Ϊ�������������
	ncDesc.FrontCounterClockwise = false;//�Ƿ�������ʱ��������������Ϊ����
	ncDesc.DepthClipEnable = true;       //������Ȳü�
	//����һ���رձ���������״̬������Ҫ�õ�ʱ������ø��豸������
	if (FAILED(device->CreateRasterizerState(&ncDesc, &noCullRS)))
	{
		MessageBox(NULL, L"Create 'NoCull' rasterizer state failed!", L"Error", MB_OK);
		return false;
	}
	//*************�ڶ�������������Ⱦ״̬************************************************

	//*************�������������벼��****************************************************
	//��GetTechniqueByName��ȡID3DX11EffectTechnique�Ķ���
	//������Ĭ�ϵ�technique��Effect
	technique = effect->GetTechniqueByName("TexTech");                //Ĭ��Technique

	//D3DX11_PASS_DESC�ṹ��������һ��Effect Pass
	D3DX11_PASS_DESC PassDesc;
	//����GetPassByIndex��ȡEffect Pass
	//������GetDesc��ȡEffect Pass��������������PassDesc������
	technique->GetPassByIndex(0)->GetDesc(&PassDesc);

	//�������������벼��
	//�������Ƕ���һ��D3D11_INPUT_ELEMENT_DESC���飬
	//�������Ƕ���Ķ���ṹ����λ������ͷ��������������������������Ԫ��
	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0,  D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"NORMAL",   0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0}
	};
	//layoutԪ�ظ���
	UINT numElements = ARRAYSIZE(layout);
	//����CreateInputLayout�������벼��
	hr = device->CreateInputLayout(layout, numElements, PassDesc.pIAInputSignature,
		PassDesc.IAInputSignatureSize, &vertexLayout);
	//�������ɵ����벼�ֵ��豸��������
	immediateContext->IASetInputLayout(vertexLayout);
	if (FAILED(hr))
	{
		::MessageBox(NULL, L"����Input Layoutʧ��", L"Error", MB_OK);
		return hr;
	}
	//*************�������������벼��****************************************************

	//*************���Ĳ��������㻺��****************************************************

	UINT vertexCount = ARRAYSIZE(vertices);
	//�������㻺�棬����ͬʵ��4һ��
	//��������һ��D3D11_BUFFER_DESC�Ķ���bd
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(Vertex) * vertexCount;      //ע�⣺�������ﶨ����24����������Ҫ����24
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;  //ע�⣺�����ʾ�������Ƕ��㻺��
	bd.CPUAccessFlags = 0;

	//����һ��D3D11_SUBRESOURCE_DATA�������ڳ�ʼ������Դ
	D3D11_SUBRESOURCE_DATA InitData;
	ZeroMemory(&InitData, sizeof(InitData));
	InitData.pSysMem = vertices;         //������Ҫ��ʼ�������ݣ���������ݾ��Ƕ�������
	InitData.SysMemPitch = 0;
	InitData.SysMemSlicePitch = 0;

	//����һ��ID3D11Buffer������Ϊ���㻺��
	ID3D11Buffer* vertexBuffer;
	//����CreateBuffer�������㻺��
	hr = device->CreateBuffer(&bd, &InitData, &vertexBuffer);
	if (FAILED(hr))
	{
		::MessageBox(NULL, L"����VertexBufferʧ��", L"Error", MB_OK);
		return hr;
	}

	UINT stride = sizeof(Vertex);                 //��ȡVertex�Ĵ�С��Ϊ���
	UINT offset = 0;                              //����ƫ����Ϊ0
	//���ö��㻺�棬�����Ľ��ͼ�ʵ��4
	immediateContext->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);
	//ָ��ͼԪ���ͣ�D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST��ʾͼԪΪ������
	immediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	//*************���Ĳ��������㻺��****************************************************

	//*************���岽���ò��ʺ͹���**************************************************
	//���ӵذ弰ǽ�Ĳ���
	floorMaterial.ambient = XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f);
	floorMaterial.diffuse = XMFLOAT4(1.f, 1.f, 1.f, 1.0f);
	floorMaterial.specular = XMFLOAT4(0.3f, 0.3f, 0.3f, 16.0f);
	floorMaterial.power = 5.0f;
	//���Ӳ���
	boxMaterial.ambient = XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f);
	boxMaterial.diffuse = XMFLOAT4(1.f, 1.f, 1.f, 1.0f);
	boxMaterial.specular = XMFLOAT4(0.3f, 0.3f, 0.3f, 16.0f);
	boxMaterial.power = 5.0f;
	//ˮ�����
	waterMaterial.ambient = XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f);
	waterMaterial.diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);   //ˮ���alphaֵΪ0.6������͸����Ϊ40%
	waterMaterial.specular = XMFLOAT4(0.3f, 0.3f, 0.3f, 16.0f);
	waterMaterial.power = 5.0f;

	// ���ù�Դ
	Light dirLight, pointLight, spotLight;
	// �����ֻ��Ҫ���ã�����3�ֹ���ǿ��
	dirLight.type = 0;
	dirLight.direction = XMFLOAT4(0.0f, -1.0f, 0.0f, 1.0f);
	dirLight.ambient = XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f);   //ǰ��λ�ֱ��ʾ���������ǿ��
	dirLight.diffuse = XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f);   //ͬ��
	dirLight.specular = XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f);  //ͬ��

	// ���Դ��Ҫ���ã�λ�á�3�й���ǿ�ȡ�3��˥������
	pointLight.type = 1;
	pointLight.position = XMFLOAT4(0.0f, 5.0f, 0.0f, 1.0f); //��Դλ��
	pointLight.ambient = XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f);   //ǰ��λ�ֱ��ʾ���������ǿ��
	pointLight.diffuse = XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f);   //ͬ��
	pointLight.specular = XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f);  //ͬ��
	pointLight.attenuation0 = 0;      //����˥������
	pointLight.attenuation1 = 0.1f;   //һ��˥������
	pointLight.attenuation2 = 0;      //����˥������

	// �۹����Ҫ����Light�ṹ�����еĳ�Ա
	spotLight.type = 2;
	spotLight.position = XMFLOAT4(0.0f, 10.0f, 0.0f, 1.0f); //��Դλ��
	spotLight.direction = XMFLOAT4(0.0f, -1.0f, 0.0f, 1.0f); //���շ���
	spotLight.ambient = XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f);   //ǰ��λ�ֱ��ʾ���������ǿ��
	spotLight.diffuse = XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f);   //ͬ��
	spotLight.specular = XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f);  //ͬ��
	spotLight.attenuation0 = 0;    //����˥������
	spotLight.attenuation1 = 0.1f; //һ��˥������
	spotLight.attenuation2 = 0;    //����˥������
	spotLight.alpha = XM_PI / 6;   //��׶�Ƕ�
	spotLight.beta = XM_PI / 3;    //��׶�Ƕ�
	spotLight.fallOff = 1.0f;      //˥��ϵ����һ��Ϊ1.0

	light[0] = dirLight;
	light[1] = pointLight;
	light[2] = spotLight;
	//*************���岽���ò��ʺ͹���**************************************************

	return true;
}

void Cleanup()
{
	//�ͷ�ȫ��ָ��
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
		static int scoreBoard[3] = { 0,0,0 };  //��ʼ����Ϸ�Ʒְ�����
		int tempScore = 0;		//����ÿ�ζ�Ҫ��score����һ�γ�����������score��static�ģ��⵼�´�����score�����ͱ��0��
							//ʹ�������ʱ����������������scoreȥ����������
	//���ӽ�ת����Ϸ��ʼ����
	if (ViewTurn == 0) {
			//����һ����������ɫ��Ϣ��4��Ԫ�طֱ��ʾ�죬�̣����Լ�alpha
			float ClearColor[4] = { 0.0f, 0.0f, 0.0f, 1.0f };
			immediateContext->ClearRenderTargetView(renderTargetView, ClearColor);
			//�����ǰ�󶨵����ģ����ͼ
			immediateContext->ClearDepthStencilView(depthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);

			//*************���ñ任����ϵ***********************	
			world = XMMatrixIdentity();
			XMVECTOR Eye = XMVectorSet(35.0f, 5.0f, -2.0f, 0.0f);
			XMVECTOR At = XMVectorSet(35.0f, 5.0f, 0.0f, 0.0f);  //Ŀ��λ��
			XMVECTOR Up = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);  //up
			view = XMMatrixLookAtLH(Eye, At, Up);   //���ù۲�����ϵ
			//����ͶӰ����
			projection = XMMatrixPerspectiveFovLH(XM_PIDIV2, 800.0f / 600.0f, 0.01f, 100.0f);
			//������任����ĳ��������еľ�����������õ�Effect�����---------------------
			effect->GetVariableByName("World")->AsMatrix()->SetMatrix((float*)&world);  //������������ϵ
			effect->GetVariableByName("View")->AsMatrix()->SetMatrix((float*)&view);    //���ù۲�����ϵ
			effect->GetVariableByName("Projection")->AsMatrix()->SetMatrix((float*)&projection); //����ͶӰ����ϵ
			effect->GetVariableByName("EyePosition")->AsMatrix()->SetMatrix((float*)&Eye); //�����ӵ�

			//��Դ�ĳ��������еĹ�Դ��Ϣ���õ�Effect�����---------------------------------
			SetLightEffect(light[lightType]);
			//*************���ñ任����ϵ***********************

			//::MessageBox(NULL, L"����ʱ��ʼ����", L"Error", MB_OK);


			//����һ��D3DX11_TECHNIQUE_DESC����������technique
			D3DX11_TECHNIQUE_DESC techDesc;
			technique->GetDesc(&techDesc);    //��ȡtechnique������
			//��ȡͨ����PASS���������õ��豸�������С�
			//��������ֻ��һ��ͨ������������Ϊ0


			//swapChain->Present(0, 0);
		}
	//���ӽ�ת����Ϸ����
	else if (ViewTurn == 1) {
		//����һ����������ɫ��Ϣ��4��Ԫ�طֱ��ʾ�죬�̣����Լ�alpha
		float ClearColor[4] = { 0.0f, 0.0f, 0.0f, 1.0f };
		immediateContext->ClearRenderTargetView(renderTargetView, ClearColor);
		//�����ǰ�󶨵����ģ����ͼ
		immediateContext->ClearDepthStencilView(depthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);

		//*************���ñ任����ϵ***********************	
		world = XMMatrixIdentity();
		XMVECTOR Eye = XMVectorSet(0.0f, 6.0f, -5.0f, 0.0f);
		XMVECTOR At = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);  //Ŀ��λ��
		XMVECTOR Up = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);  //up
		view = XMMatrixLookAtLH(Eye, At, Up);   //���ù۲�����ϵ
		//����ͶӰ����
		projection = XMMatrixPerspectiveFovLH(XM_PIDIV2, 800.0f / 600.0f, 0.01f, 100.0f);
		//������任����ĳ��������еľ�����������õ�Effect�����---------------------
		effect->GetVariableByName("World")->AsMatrix()->SetMatrix((float*)&world);  //������������ϵ
		effect->GetVariableByName("View")->AsMatrix()->SetMatrix((float*)&view);    //���ù۲�����ϵ
		effect->GetVariableByName("Projection")->AsMatrix()->SetMatrix((float*)&projection); //����ͶӰ����ϵ
		effect->GetVariableByName("EyePosition")->AsMatrix()->SetMatrix((float*)&Eye); //�����ӵ�

		//��Դ�ĳ��������еĹ�Դ��Ϣ���õ�Effect�����---------------------------------
		SetLightEffect(light[lightType]);
		//*************���ñ任����ϵ***********************

		//::MessageBox(NULL, L"����ʱ��Ϸ����", L"Error", MB_OK);

		//����һ��D3DX11_TECHNIQUE_DESC����������technique
		D3DX11_TECHNIQUE_DESC techDesc;
		technique->GetDesc(&techDesc);    //��ȡtechnique������
		//��ȡͨ����PASS���������õ��豸�������С�
		//��������ֻ��һ��ͨ������������Ϊ0


		//swapChain->Present(0, 0);
	}
	//��Ϸ���������ӽ�ת��Ƿְ����
	else {
		//����һ����������ɫ��Ϣ��4��Ԫ�طֱ��ʾ�죬�̣����Լ�alpha
		float ClearColor[4] = { 0.0f, 0.0f, 0.0f, 1.0f };
		immediateContext->ClearRenderTargetView(renderTargetView, ClearColor);
		//�����ǰ�󶨵����ģ����ͼ
		immediateContext->ClearDepthStencilView(depthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);

		//*************���ñ任����ϵ***********************	
		world = XMMatrixIdentity();
		XMVECTOR Eye = XMVectorSet(35.0f, 6.0f, -2.0f, 0.0f);
		XMVECTOR At = XMVectorSet(35.0f, 5.0f, 0.0f, 0.0f);  //Ŀ��λ��
		XMVECTOR Up = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);  //up
		view = XMMatrixLookAtLH(Eye, At, Up);   //���ù۲�����ϵ
		//����ͶӰ����
		projection = XMMatrixPerspectiveFovLH(XM_PIDIV2, 800.0f / 600.0f, 0.01f, 100.0f);
		//������任����ĳ��������еľ�����������õ�Effect�����---------------------
		effect->GetVariableByName("World")->AsMatrix()->SetMatrix((float*)&world);  //������������ϵ
		effect->GetVariableByName("View")->AsMatrix()->SetMatrix((float*)&view);    //���ù۲�����ϵ
		effect->GetVariableByName("Projection")->AsMatrix()->SetMatrix((float*)&projection); //����ͶӰ����ϵ
		effect->GetVariableByName("EyePosition")->AsMatrix()->SetMatrix((float*)&Eye); //�����ӵ�

		//��Դ�ĳ��������еĹ�Դ��Ϣ���õ�Effect�����---------------------------------
		SetLightEffect(light[lightType]);
		//*************���ñ任����ϵ***********************

		//::MessageBox(NULL, L"�����ǽ�������", L"Error", MB_OK);

		//����һ��D3DX11_TECHNIQUE_DESC����������technique
		D3DX11_TECHNIQUE_DESC techDesc;
		technique->GetDesc(&techDesc);    //��ȡtechnique������
		//��ȡͨ����PASS���������õ��豸�������С�
		//��������ֻ��һ��ͨ������������Ϊ0


		//swapChain->Present(0, 0);
	}

	//������������������������������������������������������������������������������������������������������������������������������������������������������������

		float BlendFactor[] = { 0,0,0,0 };

		//********************��һ���֣�����3������ϵ�����յ��ⲿ����****************************
		//ͨ�����̵��������Ҽ����ı���������ͷ����.
		static float angle = XM_PI;   //����һ����̬�������ڼ�¼�Ƕ�
		static float height = 2.0f;

		/*OOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO*/
		static int totalTime = 300;     //��ʼ��ʣ��ʱ��Ϊ300�룬��ʼʱ����Ϊ300ʱ����Ϸ����ʾ299s����֪��Ϊɶ
		static int timer[3] = { 0,0,0 };    //��ʼ����Ϸʱ���������

		int GoodMeatTime = 6;     //��������ɲ���Ҫ��ʱ��
		int BadMeatTime = 10;      //������������ⱻ���
		if (Cooking == 1) {     //���ڼ��⣬

			cookingTimer.Tick();  //��ʱ���⻨�ѵ�ʱ��
			if (cookingTimer.TotalTime() > GoodMeatTime) {  //�����2�����ͼ���ˣ�������
				IsMeatCookedWell = 1;

			}
			if (cookingTimer.TotalTime() > BadMeatTime) {   //�����4�����ͺ��ˣ����Զ���ʧ���޷�����ȡ
				IsMeatCookedWell = 0;
				IsOverCooked = 1;

			}
		}
		/*OOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO*/


		if (::GetAsyncKeyState(VK_LEFT) & 0x8000f) //��Ӧ���������
			angle -= 1.5f * timeDelta;
		if (::GetAsyncKeyState(VK_RIGHT) & 0x8000f) //��Ӧ�����ҷ����
			angle += 1.5f * timeDelta;
		if (::GetAsyncKeyState(VK_UP) & 0x8000f)    //��Ӧ�����Ϸ����
			height += 4.0f * timeDelta;
		if (::GetAsyncKeyState(VK_DOWN) & 0x8000f)  //��Ӧ�����·����
			height -= 4.0f * timeDelta;

		//if (height < -5.0f) height = -5.f;   //���ƾ�ͷ��Զ����
		//if (height > 5.0f) height = 5.f;     //���ƾ�ͷ�������

		//********************��һ���֣�����3������ϵ�����յ��ⲿ����****************************

		//********************�ڶ����֣����Ƹ�������*************************************************
		//**ע��**�����ƶ�������ʱ������Ȼ��Ʋ�͸�������壬�ٻ���͸�������壬
		//          ��Ϊ����Ƶ�����ᵲס�Ȼ��Ƶ����壬�����б��밴�ճ���-����-ˮ���˳�����

		D3DX11_TECHNIQUE_DESC techDesc;
		technique->GetDesc(&techDesc);    //��ȡtechnique������

		//���Ƶذ�
		//���õذ�Ĳ�����Ϣ
		effect->GetVariableByName("MatAmbient")->AsVector()->SetFloatVector((float*)&(floorMaterial.ambient));
		effect->GetVariableByName("MatDiffuse")->AsVector()->SetFloatVector((float*)&(floorMaterial.diffuse));
		effect->GetVariableByName("MatSpecular")->AsVector()->SetFloatVector((float*)&(floorMaterial.specular));
		effect->GetVariableByName("MatPower")->AsScalar()->SetFloat(floorMaterial.power);
		//���õذ������
		effect->GetVariableByName("Texture")->AsShaderResource()->SetResource(textureFloor);
		technique->GetPassByIndex(0)->Apply(0, immediateContext);
		immediateContext->Draw(6, 150);

		if (flag < 300) {
			//���ô��ʹ�����
			effect->GetVariableByName("Texture")->AsShaderResource()->SetResource(textureScrollLine);
			technique->GetPassByIndex(0)->Apply(0, immediateContext);
			immediateContext->Draw(6, 36);//���ƴ��ʹ�
			flag++;
		}
		else if (flag < 600) {
			effect->GetVariableByName("Texture")->AsShaderResource()->SetResource(textureScrollLine);
			technique->GetPassByIndex(0)->Apply(0, immediateContext);
			immediateContext->Draw(6, 228);//���ƴ��ʹ�
			flag++;
		}
		else if (flag < 900) {
			effect->GetVariableByName("Texture")->AsShaderResource()->SetResource(textureScrollLine);
			technique->GetPassByIndex(0)->Apply(0, immediateContext);
			immediateContext->Draw(6, 234);//���ƴ��ʹ�
			flag++;
		}
		else flag = 0;

		//��������
		//�������ӵĲ�����Ϣ
		effect->GetVariableByName("MatAmbient")->AsVector()->SetFloatVector((float*)&(boxMaterial.ambient));
		effect->GetVariableByName("MatDiffuse")->AsVector()->SetFloatVector((float*)&(boxMaterial.diffuse));
		effect->GetVariableByName("MatSpecular")->AsVector()->SetFloatVector((float*)&(boxMaterial.specular));
		effect->GetVariableByName("MatPower")->AsScalar()->SetFloat(boxMaterial.power);
		//�������ӵ�����
		effect->GetVariableByName("Texture")->AsShaderResource()->SetResource(textureBox);
		technique->GetPassByIndex(0)->Apply(0, immediateContext);
		immediateContext->Draw(66, 0);   //��������

		/*OOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO*/

		//ֻ����Ϸ�ڽ����У��ſ��ԼƷ�
		if (!GameOver && GameStart)
			if (IsGetScore) {//��⵽�ܹ��÷֣���������
				score = score + RewardScore;
				//::MessageBox(NULL, stringToLPCWSTR(Convert(score)), L"Over", MB_OK);     //ʱ�䵹��ʱ����������
				IsGetScore = false;
			}
		tempScore = score;

		//�������
		if (!GameStart && !GameOver) {   //�ս�����Ϸʱ��start��GameOver����false����ʱ�����ƿ�ʼ����
			//��δ��ʼ��Ϸʱ�������������Ϸ��ʼ����
			world = world * XMMatrixTranslation(35.7f, 1.5f, 0.0f);
			effect->GetVariableByName("World")->AsMatrix()->SetMatrix((float*)&world);
			effect->GetVariableByName("Texture")->AsShaderResource()->SetResource(startGame);
			technique->GetPassByIndex(0)->Apply(0, immediateContext);
			immediateContext->Draw(6, 354);
			world = XMMatrixIdentity();
			effect->GetVariableByName("World")->AsMatrix()->SetMatrix((float*)&world);  //������������ϵ
		}
		else if (GameStart && GameOver) {                 //ʱ������󣬻��ƽ������棬ͬʱҲ��������������ȥ������ʱ���н�������ͷ�����壩
			ViewTurn = 3;
			//�������������ʾ����������
			for (int i = 0; i < 3; i++) {
				scoreBoard[i] = tempScore % 10;
				tempScore /= 10;
			}

			//----------------------------------------------------------------------------------�������
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
			effect->GetVariableByName("World")->AsMatrix()->SetMatrix((float*)&world);  //������������ϵ

		}
		else {                                                         //��Ϸ�����У����Ʒ������Ͷ�ʱ���
			mytimer.Tick();  //ÿ����һ֡����ʱ����������Ӧ��ʱ��

			int leftTime = totalTime - mytimer.TotalTime();
			if (leftTime < 0) {
				//start = false;          //ʱ�䵽��start��־��Ϊfalse
				leftTime = 0;
				GameOver = true;
			}

			//����ʱ��������ʾ����������
			for (int i = 0; i < 3; i++) {
				timer[i] = leftTime % 10;
				leftTime /= 10;
			}

			//�������������ʾ����������
			for (int i = 0; i < 3; i++) {
				scoreBoard[i] = tempScore % 10;
				tempScore /= 10;
			}

			//--------------------------------------------------- �������
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

			//--------------------------------------------------- ʱ�����
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
				effect->GetVariableByName("World")->AsMatrix()->SetMatrix((float*)&world);  //������������ϵ
			}


		}
		/*OOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO*/


		//����
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

		//������
		effect->GetVariableByName("Texture")->AsShaderResource()->SetResource(textureRubbishl);
		technique->GetPassByIndex(0)->Apply(0, immediateContext);
		immediateContext->Draw(6, 264);
		effect->GetVariableByName("Texture")->AsShaderResource()->SetResource(textureRubbishr);
		technique->GetPassByIndex(0)->Apply(0, immediateContext);
		immediateContext->Draw(6, 270);
		//��
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
		//�˰�
		effect->GetVariableByName("Texture")->AsShaderResource()->SetResource(textureCuttingboard);
		technique->GetPassByIndex(0)->Apply(0, immediateContext);
		immediateContext->Draw(6, 282);
		//����
		effect->GetVariableByName("Texture")->AsShaderResource()->SetResource(textureOut);
		technique->GetPassByIndex(0)->Apply(0, immediateContext);
		immediateContext->Draw(6, 288);


		//����
		effect->GetVariableByName("MatAmbient")->AsVector()->SetFloatVector((float*)&(waterMaterial.ambient));
		effect->GetVariableByName("MatDiffuse")->AsVector()->SetFloatVector((float*)&(waterMaterial.diffuse));
		effect->GetVariableByName("MatSpecular")->AsVector()->SetFloatVector((float*)&(waterMaterial.specular));
		effect->GetVariableByName("MatPower")->AsScalar()->SetFloat(waterMaterial.power);

		//���ó��������
		effect->GetVariableByName("Texture")->AsShaderResource()->SetResource(textureDrawer);
		technique->GetPassByIndex(0)->Apply(0, immediateContext);
		immediateContext->Draw(84, 66);   //���Ƴ���

		//����ǽ����
		effect->GetVariableByName("Texture")->AsShaderResource()->SetResource(textureWall);
		technique->GetPassByIndex(0)->Apply(0, immediateContext);
		immediateContext->Draw(30, 156);//����ǽ

		//����ǽ��������
		effect->GetVariableByName("Texture")->AsShaderResource()->SetResource(textureWallTop);
		technique->GetPassByIndex(0)->Apply(0, immediateContext);
		immediateContext->Draw(30, 186);//����ǽ����

		//��������Ͱ
		effect->GetVariableByName("Texture")->AsShaderResource()->SetResource(textureRubbishl);
		technique->GetPassByIndex(0)->Apply(0, immediateContext);
		immediateContext->Draw(6, 312);   //��������Ͱ

		//�˵�����
		if (menuFlag == 1) {
			createmenu(menunum);
		}
		//----------------------------------------------------------------------------------WASD���������һ�ƶ�
		static int walkX = 0;     //��������ѡȡһ�е�����ͼƬ��Ϊ��ǰ����
		static int actSpeed = 0;  //��������ģ�����߶������ٶ�
		static float moveX = 0.0f; //X����λ����
		static float moveZ = 0.0f; //Y����λ����
		if (::GetAsyncKeyState('W') & 0x8000f) {    //��Ӧ�����Ϸ����
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
		if (::GetAsyncKeyState('S') & 0x8000f) { //S��������
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
		if (::GetAsyncKeyState('A') & 0x8000f) {    //A��������
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
		if (::GetAsyncKeyState('D') & 0x8000f) {  //D��������
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
			if (moveZ < -3.0f)moveZ = -2.9f;  //����£�����������߽�
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
		//----------------------------------------------------------------------------------WASD���������һ�ƶ�

		//----------------------------------------------------------------------------------IJKL���������2�ƶ�
		static int walkX2 = 0;     //��������ѡȡһ�е�����ͼƬ��Ϊ��ǰ����
		static int actSpeed2 = 0;  //��������ģ�����߶������ٶ�
		static float moveX2 = 0.0f; //X����λ����
		static float moveZ2 = 0.0f; //Y����λ����
		if (::GetAsyncKeyState(VK_UP) & 0x8000f) {    //��Ӧ�����Ϸ����
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
		if (::GetAsyncKeyState(VK_DOWN) & 0x8000f) { //�·����������
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
		if (::GetAsyncKeyState(VK_RIGHT) & 0x8000f) {    //�ҷ����������
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
		if (::GetAsyncKeyState(VK_LEFT) & 0x8000f) {  //�����������
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

		/*==================��ײ���*/
		if (moveZ2 >= 0.0f) {
			if (moveZ2 > 2.8f)	moveZ2 = 2.8f;  //����£�����������߽�
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

		//----------------------------------------------------------------------------------IJKL���������2�ƶ�

		//-----------------------------------------------------------------------------------���¼���������
		//����HRESULT�Ķ������ڼ�¼���������Ƿ�ɹ�
		hr = S_OK;
		vertexCount = ARRAYSIZE(vertices);
		//�������㻺�棬����ͬʵ��4һ��
		ZeroMemory(&bd, sizeof(bd));
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.ByteWidth = sizeof(Vertex) * vertexCount;
		bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;  //ע�⣺�����ʾ�������Ƕ��㻺��
		bd.CPUAccessFlags = 0;
		ZeroMemory(&InitData, sizeof(InitData));
		InitData.pSysMem = vertices;         //������Ҫ��ʼ�������ݣ���������ݾ��Ƕ�������
		//����CreateBuffer�������㻺��
		if (DrawTime > 0)
			vertexBuffer->Release();
		hr = device->CreateBuffer(&bd, &InitData, &vertexBuffer);

		if (FAILED(hr))
		{
			::MessageBox(NULL, L"����VertexBufferʧ��", L"Error", MB_OK);
			return hr;
		}

		UINT stride = sizeof(Vertex);                 //��ȡVertex�Ĵ�С��Ϊ���
		UINT offset = 0;                              //����ƫ����Ϊ0
		//���ö��㻺�棬�����Ľ��ͼ�ʵ��4
		immediateContext->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);
		DrawTime = 1;
		//---------------------------------------------------------------------------------------���¼���������

		world = XMMatrixIdentity();
		world = XMMatrixTranslation(moveX, 0.0f, moveZ);
		effect->GetVariableByName("World")->AsMatrix()->SetMatrix((float*)&world);  //������������ϵ

		immediateContext->OMSetBlendState(blendStateAlpha, BlendFactor, 0xffffffff);  //�������
		immediateContext->RSSetState(noCullRS);                                       //�رձ�������

		//world = XMMatrixIdentity();
		effect->GetVariableByName("MatAmbient")->AsVector()->SetFloatVector((float*)&(waterMaterial.ambient));
		effect->GetVariableByName("MatDiffuse")->AsVector()->SetFloatVector((float*)&(waterMaterial.diffuse));
		effect->GetVariableByName("MatSpecular")->AsVector()->SetFloatVector((float*)&(waterMaterial.specular));
		effect->GetVariableByName("MatPower")->AsScalar()->SetFloat(waterMaterial.power);
		//��������1������
		effect->GetVariableByName("Texture")->AsShaderResource()->SetResource(texturePlayer1);
		technique->GetPassByIndex(0)->Apply(0, immediateContext);
		immediateContext->Draw(6, 216);

		//�������ǻ����ж�
		{

			//�ո���Ļ����жϣ�������������ڱ߽磬����£�����ޣ����ж�λ�ã�����������ȡ�
			if (::GetAsyncKeyState(VK_SPACE) & 0x8000f)
			{
				CanCookMeat(moveX, moveZ);
				CanGetFood(moveX, moveZ);
				CanPutInScroll(moveX, moveZ);
				CanThrowFood(moveX, moveZ);
			}
			//Enter�Ļ����жϣ�������������ڱ߽磬����£�����ޣ����ж�λ�ã�����������ȡ�
			//�жϵ����ϲ������ȡ�����
			if (::GetAsyncKeyState(VK_RETURN) & 0x8000f)
			{
				CanGetFromScroll(moveX2, moveZ2);
				CanCutVegtable(moveX2, moveZ2);
				SendOutFood(moveX2, moveZ2);
			}

		}
		//�����жϽ���

		//����֮�������ı��Լ��˶�
		//ָʾ�������ͷ�����Ƿ�Ӧ����ʾ��õ���
		if (IsMeatWell[0] == 1)
			setTextureOfPlayerHead(300, moveX, moveZ, textureMeatCooked);
		//ָʾ�������ͷ�����Ƿ�Ӧ����ʾ�кõ�������
		if (IsTomatoWell[1] == 1)
			setTextureOfPlayerHead(306, -moveX2, moveZ2, textureTomatoCutted);
		//ָʾ�������ͷ�����Ƿ�Ӧ����ʾ�кõ�����
		if (IsLettuceWell[1] == 1)
			setTextureOfPlayerHead(306, -moveX2, moveZ2, textureLettuceCutted);
		//ָʾ�������ͷ�����Ƿ�Ӧ����ʾ�кõ����
		if (IsBreadWell[1] == 1)
			setTextureOfPlayerHead(306, -moveX2, moveZ2, textureBreadCutted);
		//ֻ���������ͷ�����Ƿ�Ӧ����ʾ��õ���
		if (IsMeatWell[1] == 1)
			setTextureOfPlayerHead(306, -moveX2, moveZ2, textureMeatCooked);
		//ȡ�����
		if (IsBread[0] == 1)
			setTextureOfPlayerHead(300, moveX, moveZ, textureHeadbread);
		//ȡ����
		if (IsMeat[0] == 1)
			setTextureOfPlayerHead(300, moveX, moveZ, textureHeadmeat);
		//ȡ��������
		if (IsTomato[0] == 1)
			setTextureOfPlayerHead(300, moveX, moveZ, textureHeadtomato);
		//ȡ���߲�
		if (IsLettuce[0] == 1)
			setTextureOfPlayerHead(300, moveX, moveZ, textureHeadvegetable);
		//ȡ����������
		if (IsHaveOverCookedMeat == 1)
			setTextureOfPlayerHead(300, moveX, moveZ, textureOverCookedMeat);

		if (IsBread[1] == 1)
			setTextureOfPlayerHead(306, -moveX2, moveZ2, textureHeadbread);
		if (IsLettuce[1] == 1)
			setTextureOfPlayerHead(306, -moveX2, moveZ2, textureHeadvegetable);
		if (IsTomato[1] == 1)
			setTextureOfPlayerHead(306, -moveX2, moveZ2, textureHeadtomato);
		//����ŵ����ʹ���
		if (IsBreadInSC == 1)
			putFoodInScroll(1, PutToSCXMove, 7.0f, timeDelta, 300, textureHeadbread);
		//��ŵ����ʹ���
		if (IsMeatWellInSC == 1)
			putFoodInScroll(1, PutToSCXMove, 7.0f, timeDelta, 300, textureMeatCooked);
		//�������ŵ����ʹ���
		if (IsTomatoInSC == 1)
			putFoodInScroll(1, PutToSCXMove, 7.0f, timeDelta, 300, textureHeadtomato);
		//�߲˷ŵ����ʹ���
		if (IsLettuceInSC == 1)
			putFoodInScroll(1, PutToSCXMove, 7.0f, timeDelta, 300, textureHeadvegetable);

		/*************************************************************/
		world = XMMatrixIdentity();
		world = XMMatrixTranslation(-moveX2, 0.0f, moveZ2);
		effect->GetVariableByName("World")->AsMatrix()->SetMatrix((float*)&world);  //������������ϵ
		effect->GetVariableByName("MatAmbient")->AsVector()->SetFloatVector((float*)&(waterMaterial.ambient));
		effect->GetVariableByName("MatDiffuse")->AsVector()->SetFloatVector((float*)&(waterMaterial.diffuse));
		effect->GetVariableByName("MatSpecular")->AsVector()->SetFloatVector((float*)&(waterMaterial.specular));
		effect->GetVariableByName("MatPower")->AsScalar()->SetFloat(waterMaterial.power);

		//��������2������
		effect->GetVariableByName("Texture")->AsShaderResource()->SetResource(texturePlayer2);
		technique->GetPassByIndex(0)->Apply(0, immediateContext);
		immediateContext->Draw(6, 222);
		immediateContext->OMSetBlendState(0, 0, 0xffffffff);   //�رջ�ϣ���ס������رջ��
		immediateContext->RSSetState(0);                     //�ָ���������
		//********************�ڶ����֣����Ƹ�������*************************************************

		swapChain->Present(0, 0);
	}
	return true;
}
//**************��ܺ���******************

// �����ﵱǰ���괦��ʳ����õ����ʹ���
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
	effect->GetVariableByName("World")->AsMatrix()->SetMatrix((float*)&world);  //������������ϵ
	effect->GetVariableByName("MatAmbient")->AsVector()->SetFloatVector((float*)&(waterMaterial.ambient));
	effect->GetVariableByName("MatDiffuse")->AsVector()->SetFloatVector((float*)&(waterMaterial.diffuse));
	effect->GetVariableByName("MatSpecular")->AsVector()->SetFloatVector((float*)&(waterMaterial.specular));
	effect->GetVariableByName("MatPower")->AsScalar()->SetFloat(waterMaterial.power);

	effect->GetVariableByName("Texture")->AsShaderResource()->SetResource(textureTemp);
	technique->GetPassByIndex(0)->Apply(0, immediateContext);
	immediateContext->Draw(6, vertexNums);
}

//�ı�����ͷ�����ӵ�����
void setTextureOfPlayerHead(int vertexNums, float tmpMoveX, float tmpMoveZ, ID3D11ShaderResourceView* textureTempChange)
{
	world = XMMatrixIdentity();
	world = XMMatrixTranslation(tmpMoveX, 0.0f, tmpMoveZ);
	effect->GetVariableByName("World")->AsMatrix()->SetMatrix((float*)&world);  //������������ϵ 

	effect->GetVariableByName("MatAmbient")->AsVector()->SetFloatVector((float*)&(waterMaterial.ambient));
	effect->GetVariableByName("MatDiffuse")->AsVector()->SetFloatVector((float*)&(waterMaterial.diffuse));
	effect->GetVariableByName("MatSpecular")->AsVector()->SetFloatVector((float*)&(waterMaterial.specular));
	effect->GetVariableByName("MatPower")->AsScalar()->SetFloat(waterMaterial.power);
	effect->GetVariableByName("Texture")->AsShaderResource()->SetResource(textureTempChange);
	technique->GetPassByIndex(0)->Apply(0, immediateContext);
	immediateContext->Draw(6, vertexNums);
	//MessageBox(NULL, L"Get1111111111!", L"get food OK", MB_OK);
}

//�ж������Ƿ񵽴�ȡ�ĵص�
bool CanGetFood(float x, float z)
{
	if (IsBread[0] == 0 && IsTomato[0] == 0 && IsMeat[0] == 0 && IsLettuce[0] == 0 && IsBreadWell[0] == 0 && IsTomatoWell[0] == 0 && IsMeatWell[0] == 0 && IsLettuceWell[0] == 0 && IsHaveOverCookedMeat == 0)
		//�ж��·�����Ļ�ȡ�߼�
	{
		//�����ӻ�ȡʳ��
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
		//�Ӽ����ȡʳ��
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
						cookingTimer.Stop();    //�ⱻȡ�ߺ�ȡ�ⶨʱ��ֹͣ����
						return true;
					}
					if (IsOverCooked == 1)
					{
						Cooking = 0;
						IsMeatCookedWell = 0;
						IsOverCooked = 0;
						IsHaveOverCookedMeat = 1;
						cookingTimer.Stop();    //�ⱻȡ�ߺ�ȡ�ⶨʱ��ֹͣ����
					}
				}
			}
		}
	}
	//�ж��Ϸ�����Ļ�ȡ�߼�
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
				/*       �������зŵ���ʼ���⣨����1����ŵ������ʱ�򣩵��ж���        */
				cookingTimer.Reset();//���Լ����ˣ�������ʱ����ʼ��ʱ
			/*       �������зŵ���ʼ���⣨����1����ŵ������ʱ�򣩵��ж���        */
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
					//�ȴ�һ��ʱ����߲��ȴ�//���������ƶ�
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
		if (z <= -0.8 && x <= -PutToSCXMove - 2.0f && x >= -PutToSCXMove - 3.0f)//��ȡ���ʹ��ϵĲ�
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
	if (MenuKind == 0)//��������ˣ�����
	{
		if (FoodVarity[0] >= 1 && FoodVarity[1] >= 1 && FoodVarity[2] >= 1)
		{
			for (int i = 0; i < 3; i++)
				FoodVarity[i] -= 1;
			//MessageBox(NULL, L"�����Ʒ", L"YES", MB_OK);
			PlaySound(L"music/FinishCooking.wav", NULL, SND_FILENAME | SND_ASYNC);
			IsGetScore = true;
			srand((int)time(0));
			menunum = rand() % 999;
			return true;
		}
	}
	if (MenuKind == 1)//��������ˣ���
	{
		if (FoodVarity[0] >= 1 && FoodVarity[1] >= 1 && FoodVarity[3] >= 1)
		{
			for (int i = 0; i < 2; i++)
				FoodVarity[i] -= 1;
			FoodVarity[3] -= 1;
			//MessageBox(NULL, L"�����Ʒ", L"YES", MB_OK);
			PlaySound(L"music/FinishCooking.wav", NULL, SND_FILENAME | SND_ASYNC);
			IsGetScore = true;
			srand((int)time(0));
			menunum = rand() % 999;
			return true;
		}
	}
	if (MenuKind == 2) //��������ѣ���
	{
		if (FoodVarity[0] >= 1 && FoodVarity[2] >= 1 && FoodVarity[3] >= 1)
		{
			for (int i = 2; i < 4; i++)
				FoodVarity[i] -= 1;
			FoodVarity[0] -= 1;
			//MessageBox(NULL, L"�����Ʒ", L"YES", MB_OK);
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
		//��������ˣ�����

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
		//��������ˣ���

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
		//��������ѣ���

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
// �ص�����
//
LRESULT CALLBACK d3d::WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_DESTROY:
		::PostQuitMessage(0);
		break;

	case WM_KEYDOWN:
		if (wParam == 'F') {   //�س���������Ϸ����ʱ
			menuFlag = 1;
			ViewTurn = 1;
			GameStart = true;
			mytimer.Reset();
		}
		if (wParam == VK_ESCAPE)
			::DestroyWindow(hwnd);
		if (wParam == VK_F1)  //��F1������Դ���͸�Ϊ�����
			lightType = 0;
		if (wParam == VK_F2)  //��F2������Դ���͸�Ϊ���Դ
			lightType = 1;
		if (wParam == VK_F3)  //��F3������Դ���͸�Ϊ�۹�ƹ�Դ
			lightType = 2;
		break;
	}
	return ::DefWindowProc(hwnd, msg, wParam, lParam);
}

//
// ������WinMain
//
int WINAPI WinMain(HINSTANCE hinstance, HINSTANCE prevInstance, PSTR cmdLine, int showCmd)
{

	//��ʼ��
	//**ע��**:������������IDirect3DDevice9ָ�룬��������Ϊ��������InitD3D����
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

//��Դ�ĳ����������õ�Effect�����
//���ڹ������ñȽϸ��ӣ�������һ����������������
void SetLightEffect(Light light)
{
	//���Ƚ��������ͣ�������ǿ�ȣ������ǿ�ȣ������ǿ�����õ�Effect��
	effect->GetVariableByName("type")->AsScalar()->SetInt(light.type);
	effect->GetVariableByName("LightAmbient")->AsVector()->SetFloatVector((float*)&(light.ambient));
	effect->GetVariableByName("LightDiffuse")->AsVector()->SetFloatVector((float*)&(light.diffuse));
	effect->GetVariableByName("LightSpecular")->AsVector()->SetFloatVector((float*)&(light.specular));

	//������ݹ������͵Ĳ�ͬ���ò�ͬ������
	if (light.type == 0)  //�����
	{
		//�����ֻ��Ҫ������������Լ���
		effect->GetVariableByName("LightDirection")->AsVector()->SetFloatVector((float*)&(light.direction));
		//��������Tectnique���õ�Effect
		technique = effect->GetTechniqueByName("T_DirLight");
	}
	else if (light.type == 1)  //���Դ
	{
		//���Դ��Ҫ��λ�á���������˥�����ӡ�����һ��˥�����ӡ���������˥�����ӡ�
		effect->GetVariableByName("LightPosition")->AsVector()->SetFloatVector((float*)&(light.position));
		effect->GetVariableByName("LightAtt0")->AsScalar()->SetFloat(light.attenuation0);
		effect->GetVariableByName("LightAtt1")->AsScalar()->SetFloat(light.attenuation1);
		effect->GetVariableByName("LightAtt2")->AsScalar()->SetFloat(light.attenuation2);

		//�����Դ��Tectnique���õ�Effect
		technique = effect->GetTechniqueByName("T_PointLight");
	}
	else if (light.type == 2) //�۹�ƹ�Դ
	{
		//���Դ��Ҫ�����򡱣������򡱣�������˥�����ӡ�����һ��˥�����ӡ���������˥�����ӡ�
		//����׶�Ƕȡ�������׶�Ƕȡ������۹��˥��ϵ����
		effect->GetVariableByName("LightPosition")->AsVector()->SetFloatVector((float*)&(light.position));
		effect->GetVariableByName("LightDirection")->AsVector()->SetFloatVector((float*)&(light.direction));

		effect->GetVariableByName("LightAtt0")->AsScalar()->SetFloat(light.attenuation0);
		effect->GetVariableByName("LightAtt1")->AsScalar()->SetFloat(light.attenuation1);
		effect->GetVariableByName("LightAtt2")->AsScalar()->SetFloat(light.attenuation2);

		effect->GetVariableByName("LightAlpha")->AsScalar()->SetFloat(light.alpha);
		effect->GetVariableByName("LightBeta")->AsScalar()->SetFloat(light.beta);
		effect->GetVariableByName("LightFallOff")->AsScalar()->SetFloat(light.fallOff);

		//���۹�ƹ�Դ��Tectnique���õ�Effect
		technique = effect->GetTechniqueByName("T_SpotLight");
	}
}

