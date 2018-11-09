#pragma  once
#include	"Pt_MathTable.h"
#include	<stdlib.h>
#include	<string.h>
#include	<math.h>
#define     __IN
#define     __OUT
#define		PT_FAILED			0
#define		PT_SUCCEEDED		1
#define     PT_FALSE			0
#define     PT_TRUE				1
#define		PT_NULL				0
#define     PT_PI				3.141592653F




typedef		void				pt_void;
typedef		int					pt_bool;
typedef		unsigned int		pt_dword;
typedef     unsigned short		pt_word;
typedef		unsigned short		pt_ushort;
typedef     unsigned int		pt_uint;
typedef     int					pt_int;
typedef		char				pt_char;
typedef		char				pt_byte;
typedef		unsigned char		pt_uchar;
typedef		unsigned long		pt_ulong;
typedef		long				pt_long;
typedef     float				pt_float;
typedef     double				pt_double;


pt_char * __PT_STRCPY(pt_char *dest,const pt_char *Source);
pt_char * __PT_STRCAT(pt_char *dest,const pt_char *Source);
pt_char * __PT_ITOA(_In_ pt_int _Val, _Pre_notnull_ _Post_z_ pt_char * _DstBuf, _In_ pt_int _Radix);
#define PT_STRCPY(x,y) __PT_STRCPY(x,y)
#define PT_STRCAT(x,y) __PT_STRCAT(x,y)
#define PT_ITOA(x,y,z) __PT_ITOA(x,y,z)
struct pt_matrix 
{
public:
	pt_matrix()
	{
		m[0][0]=0;m[0][1]=0;m[0][2]=0;m[0][3]=0;
		m[1][0]=0;m[1][1]=0;m[1][2]=0;m[1][3]=0;
		m[2][0]=0;m[2][1]=0;m[2][2]=0;m[2][3]=0;
		m[3][0]=0;m[3][1]=0;m[3][2]=0;m[3][3]=0;
	}
	union {
		struct {
			pt_float        _11, _12, _13, _14;
			pt_float        _21, _22, _23, _24;
			pt_float        _31, _32, _33, _34;
			pt_float        _41, _42, _43, _44;

		};
		pt_float m[4][4];
	};
};

struct Pt_Matrix : public pt_matrix
{
public:
    Pt_Matrix() {};
    Pt_Matrix( const pt_matrix& ptmat)
	{
		this->_11=ptmat._11;this->_12=ptmat._12;this->_13=ptmat._13;this->_14=ptmat._14;
		this->_21=ptmat._21;this->_22=ptmat._22;this->_23=ptmat._23;this->_24=ptmat._24;
		this->_31=ptmat._31;this->_32=ptmat._32;this->_33=ptmat._33;this->_34=ptmat._34;
		this->_41=ptmat._41;this->_42=ptmat._42;this->_43=ptmat._43;this->_44=ptmat._44;
	}
    Pt_Matrix( pt_float _11, pt_float _12, pt_float _13, pt_float _14,
                pt_float _21, pt_float _22, pt_float _23, pt_float _24,
                pt_float _31, pt_float _32, pt_float _33, pt_float _34,
                pt_float _41, pt_float _42, pt_float _43, pt_float _44 )
	{
		this->_11=_11;this->_12=_12;this->_13=_13;this->_14=_14;
		this->_21=_21;this->_22=_22;this->_23=_23;this->_24=_24;
		this->_31=_31;this->_32=_32;this->_33=_33;this->_34=_34;
		this->_41=_41;this->_42=_42;this->_43=_43;this->_44=_44;
	}
	Pt_Matrix(float *fl)
	{
		this->_11=fl[0];this->_12=fl[1];this->_13=fl[2];this->_14=fl[3];
		this->_21=fl[4];this->_22=fl[5];this->_23=fl[6];this->_24=fl[7];
		this->_31=fl[8];this->_32=fl[9];this->_33=fl[10];this->_34=fl[11];
		this->_41=fl[12];this->_42=fl[13];this->_43=fl[14];this->_44=fl[15];
	}
    // access grants
    pt_float& operator () ( pt_uint Row, pt_uint Col ){return m[Row][Col];}
    pt_float  operator () ( pt_uint Row, pt_uint Col ) const{return m[Row][Col];}

    // casting operators
    operator pt_float* (){return (pt_float *)this;};
    operator const pt_float* () const{return (const pt_float *)this;}


    // assignment operators
    Pt_Matrix& operator *= ( Pt_Matrix mat)
	{
		Pt_Matrix ptmat;
		ptmat.m[0][0]=this->m[0][0]*mat.m[0][0]/**/+m[0][1]*mat.m[1][0]/**/+m[0][2]*mat.m[2][0]/**/+m[0][3]*mat.m[3][0];
		ptmat.m[0][1]=this->m[0][0]*mat.m[0][1]/**/+m[0][1]*mat.m[1][1]/**/+m[0][2]*mat.m[2][1]/**/+m[0][3]*mat.m[3][1];
			ptmat.m[0][2]=this->m[0][0]*mat.m[0][2]/**/+m[0][1]*mat.m[1][2]/**/+m[0][2]*mat.m[2][2]/**/+m[0][3]*mat.m[3][2];
		ptmat.m[0][3]=this->m[0][0]*mat.m[0][3]/**/+m[0][1]*mat.m[1][3]/**/+m[0][2]*mat.m[2][3]/**/+m[0][3]*mat.m[3][3];
		
		ptmat.m[1][0]=this->m[1][0]*mat.m[0][0]/**/+m[1][1]*mat.m[1][0]/**/+m[1][2]*mat.m[2][0]/**/+m[1][3]*mat.m[3][0];
		ptmat.m[1][1]=this->m[1][0]*mat.m[0][1]/**/+m[1][1]*mat.m[1][1]/**/+m[1][2]*mat.m[2][1]/**/+m[1][3]*mat.m[3][1];
		ptmat.m[1][2]=this->m[1][0]*mat.m[0][2]/**/+m[1][1]*mat.m[1][2]/**/+m[1][2]*mat.m[2][2]/**/+m[1][3]*mat.m[3][2];
		ptmat.m[1][3]=this->m[1][0]*mat.m[0][3]/**/+m[1][1]*mat.m[1][3]/**/+m[1][2]*mat.m[2][3]/**/+m[1][3]*mat.m[3][3];

		ptmat.m[2][0]=this->m[2][0]*mat.m[0][0]/**/+m[2][1]*mat.m[1][0]/**/+m[2][2]*mat.m[2][0]/**/+m[2][3]*mat.m[3][0];
		ptmat.m[2][1]=this->m[2][0]*mat.m[0][1]/**/+m[2][1]*mat.m[1][1]/**/+m[2][2]*mat.m[2][1]/**/+m[2][3]*mat.m[3][1];
		ptmat.m[2][2]=this->m[2][0]*mat.m[0][2]/**/+m[2][1]*mat.m[1][2]/**/+m[2][2]*mat.m[2][2]/**/+m[2][3]*mat.m[3][2];
		ptmat.m[2][3]=this->m[2][0]*mat.m[0][3]/**/+m[2][1]*mat.m[1][3]/**/+m[2][2]*mat.m[2][3]/**/+m[2][3]*mat.m[3][3];

		ptmat.m[3][0]=this->m[3][0]*mat.m[0][0]/**/+m[3][1]*mat.m[1][0]/**/+m[3][2]*mat.m[2][0]/**/+m[3][3]*mat.m[3][0];
		ptmat.m[3][1]=this->m[3][0]*mat.m[0][1]/**/+m[3][1]*mat.m[1][1]/**/+m[3][2]*mat.m[2][1]/**/+m[3][3]*mat.m[3][1];
		ptmat.m[3][2]=this->m[3][0]*mat.m[0][2]/**/+m[3][1]*mat.m[1][2]/**/+m[3][2]*mat.m[2][2]/**/+m[3][3]*mat.m[3][2];
		ptmat.m[3][3]=this->m[3][0]*mat.m[0][3]/**/+m[3][1]*mat.m[1][3]/**/+m[3][2]*mat.m[2][3]/**/+m[3][3]*mat.m[3][3];
		*this=ptmat;
		return *this;
	}
	;
	Pt_Matrix& operator += ( const Pt_Matrix& mat)
	{
		this->_11+=mat._11;this->_12+=mat._12;this->_13+=mat._13;this->_14+=mat._14;
		this->_21+=mat._21;this->_22+=mat._22;this->_23+=mat._23;this->_24+=mat._24;
		this->_31+=mat._31;this->_32+=mat._32;this->_33+=mat._33;this->_34+=mat._34;
		this->_41+=mat._41;this->_42+=mat._42;this->_43+=mat._43;this->_44+=mat._44;
		return *this;
	}
    Pt_Matrix& operator -= ( const Pt_Matrix& mat)
	{
		this->_11-=mat._11;this->_12-=mat._12;this->_13-=mat._13;this->_14-=mat._14;
		this->_21-=mat._21;this->_22-=mat._22;this->_23-=mat._23;this->_24-=mat._24;
		this->_31-=mat._31;this->_32-=mat._32;this->_33-=mat._33;this->_34-=mat._34;
		this->_41-=mat._41;this->_42-=mat._42;this->_43-=mat._43;this->_44-=mat._44;
		return *this;
	}

    Pt_Matrix& operator *= ( pt_float fl )
	{
		m[0][0]*=fl;m[0][1]*=fl;m[0][2]*=fl;m[0][3]*=fl;
		m[1][0]*=fl;m[1][1]*=fl;m[1][2]*=fl;m[1][3]*=fl;
		m[2][0]*=fl;m[2][1]*=fl;m[2][2]*=fl;m[2][3]*=fl;
		m[3][0]*=fl;m[3][1]*=fl;m[3][2]*=fl;m[3][3]*=fl;
		return *this;
	}
    Pt_Matrix& operator /= ( pt_float fl )
	{
		m[0][0]/=fl;m[0][1]/=fl;m[0][2]/=fl;m[0][3]/=fl;
		m[1][0]/=fl;m[1][1]/=fl;m[1][2]/=fl;m[1][3]/=fl;
		m[2][0]/=fl;m[2][1]/=fl;m[2][2]/=fl;m[2][3]/=fl;
		m[3][0]/=fl;m[3][1]/=fl;m[3][2]/=fl;m[3][3]/=fl;
		return *this;
	}

    // unary operators

    // binary operators
    Pt_Matrix operator * ( const Pt_Matrix& mat) const
	{
		Pt_Matrix ptmat;
		ptmat.m[0][0]=this->m[0][0]*mat.m[0][0]/**/+m[0][1]*mat.m[1][0]/**/+m[0][2]*mat.m[2][0]/**/+m[0][3]*mat.m[3][0];
		ptmat.m[0][1]=this->m[0][0]*mat.m[0][1]/**/+m[0][1]*mat.m[1][1]/**/+m[0][2]*mat.m[2][1]/**/+m[0][3]*mat.m[3][1];
		ptmat.m[0][2]=this->m[0][0]*mat.m[0][2]/**/+m[0][1]*mat.m[1][2]/**/+m[0][2]*mat.m[2][2]/**/+m[0][3]*mat.m[3][2];
		ptmat.m[0][3]=this->m[0][0]*mat.m[0][3]/**/+m[0][1]*mat.m[1][3]/**/+m[0][2]*mat.m[2][3]/**/+m[0][3]*mat.m[3][3];

		ptmat.m[1][0]=this->m[1][0]*mat.m[0][0]/**/+m[1][1]*mat.m[1][0]/**/+m[1][2]*mat.m[2][0]/**/+m[1][3]*mat.m[3][0];
		ptmat.m[1][1]=this->m[1][0]*mat.m[0][1]/**/+m[1][1]*mat.m[1][1]/**/+m[1][2]*mat.m[2][1]/**/+m[1][3]*mat.m[3][1];
		ptmat.m[1][2]=this->m[1][0]*mat.m[0][2]/**/+m[1][1]*mat.m[1][2]/**/+m[1][2]*mat.m[2][2]/**/+m[1][3]*mat.m[3][2];
		ptmat.m[1][3]=this->m[1][0]*mat.m[0][3]/**/+m[1][1]*mat.m[1][3]/**/+m[1][2]*mat.m[2][3]/**/+m[1][3]*mat.m[3][3];

		ptmat.m[2][0]=this->m[2][0]*mat.m[0][0]/**/+m[2][1]*mat.m[1][0]/**/+m[2][2]*mat.m[2][0]/**/+m[2][3]*mat.m[3][0];
		ptmat.m[2][1]=this->m[2][0]*mat.m[0][1]/**/+m[2][1]*mat.m[1][1]/**/+m[2][2]*mat.m[2][1]/**/+m[2][3]*mat.m[3][1];
		ptmat.m[2][2]=this->m[2][0]*mat.m[0][2]/**/+m[2][1]*mat.m[1][2]/**/+m[2][2]*mat.m[2][2]/**/+m[2][3]*mat.m[3][2];
		ptmat.m[2][3]=this->m[2][0]*mat.m[0][3]/**/+m[2][1]*mat.m[1][3]/**/+m[2][2]*mat.m[2][3]/**/+m[2][3]*mat.m[3][3];
		
		ptmat.m[3][0]=this->m[3][0]*mat.m[0][0]/**/+m[3][1]*mat.m[1][0]/**/+m[3][2]*mat.m[2][0]/**/+m[3][3]*mat.m[3][0];
		ptmat.m[3][1]=this->m[3][0]*mat.m[0][1]/**/+m[3][1]*mat.m[1][1]/**/+m[3][2]*mat.m[2][1]/**/+m[3][3]*mat.m[3][1];
		ptmat.m[3][2]=this->m[3][0]*mat.m[0][2]/**/+m[3][1]*mat.m[1][2]/**/+m[3][2]*mat.m[2][2]/**/+m[3][3]*mat.m[3][2];
		ptmat.m[3][3]=this->m[3][0]*mat.m[0][3]/**/+m[3][1]*mat.m[1][3]/**/+m[3][2]*mat.m[2][3]/**/+m[3][3]*mat.m[3][3];
		return ptmat;
	}
    Pt_Matrix operator + ( const Pt_Matrix& mat) const
	{
		Pt_Matrix ptmat;
		ptmat._11=this->_11+mat._11;ptmat._12=this->_12+mat._12;ptmat._13=this->_13+mat._13;ptmat._14=this->_14+mat._14;
		ptmat._21=this->_21+mat._21;ptmat._22=this->_22+mat._22;ptmat._23=this->_23+mat._23;ptmat._24=this->_24+mat._24;
		ptmat._31=this->_31+mat._31;ptmat._32=this->_32+mat._32;ptmat._33=this->_33+mat._33;ptmat._34=this->_34+mat._34;
		ptmat._41=this->_41+mat._41;ptmat._42=this->_42+mat._42;ptmat._43=this->_43+mat._43;ptmat._44=this->_44+mat._44;
		return ptmat;
	}
    Pt_Matrix operator - ( const Pt_Matrix& mat) const
	{
		Pt_Matrix ptmat;
		ptmat._11=this->_11-mat._11;ptmat._12=this->_12-mat._12;ptmat._13=this->_13-mat._13;ptmat._14=this->_14-mat._14;
		ptmat._21=this->_21-mat._21;ptmat._22=this->_22-mat._22;ptmat._23=this->_23-mat._23;ptmat._24=this->_24-mat._24;
		ptmat._31=this->_31-mat._31;ptmat._32=this->_32-mat._32;ptmat._33=this->_33-mat._33;ptmat._34=this->_34-mat._34;
		ptmat._41=this->_41-mat._41;ptmat._42=this->_42-mat._42;ptmat._43=this->_43-mat._43;ptmat._44=this->_44-mat._44;
		return ptmat;
	}
    Pt_Matrix operator * ( pt_float fl ) const
	{
		Pt_Matrix ptmat;
		ptmat._11=this->_11*fl;ptmat._12=this->_12*fl;ptmat._13=this->_13*fl;ptmat._14=this->_14*fl;
		ptmat._21=this->_21*fl;ptmat._22=this->_22*fl;ptmat._23=this->_23*fl;ptmat._24=this->_24*fl;
		ptmat._31=this->_31*fl;ptmat._32=this->_32*fl;ptmat._33=this->_33*fl;ptmat._34=this->_34*fl;
		ptmat._41=this->_41*fl;ptmat._42=this->_42*fl;ptmat._43=this->_43*fl;ptmat._44=this->_44*fl;
		return ptmat;
	} 
	


    Pt_Matrix operator / ( pt_float fl) const
	{
		Pt_Matrix ptmat;
		ptmat._11=this->_11/fl;ptmat._12=this->_12/fl;ptmat._13=this->_13/fl;ptmat._14=this->_14/fl;
		ptmat._21=this->_21/fl;ptmat._22=this->_22/fl;ptmat._23=this->_23/fl;ptmat._24=this->_24/fl;
		ptmat._31=this->_31/fl;ptmat._32=this->_32/fl;ptmat._33=this->_33/fl;ptmat._34=this->_34/fl;
		ptmat._41=this->_41/fl;ptmat._42=this->_42/fl;ptmat._43=this->_43/fl;ptmat._44=this->_44/fl;
		return ptmat;
	}

    pt_bool operator == ( const Pt_Matrix& mat) const
	{
		for (int i=0;i<4;i++)
		{
			for (int j=0;j<4;j++)
			{
				if (m[i][j]!=mat.m[i][j])
				{
					return PT_FALSE;
				}
			}
		}
		return PT_TRUE;
	}
    pt_bool operator != ( const Pt_Matrix& mat) const
	{
		return !(*this==mat);
	}

	void Identity()
	{
		this->_11=1.0f;	this->_12=0.0f;	this->_13=0.0f;	this->_14=0.0f;
		this->_21=0.0f;	this->_22=1.0f;	this->_23=0.0f;	this->_24=0.0f;
		this->_31=0.0f;	this->_32=0.0f;	this->_33=1.0f;	this->_34=0.0f;
		this->_41=0.0f;	this->_42=0.0f;	this->_43=0.0f;	this->_44=1.0f;
	}

	Pt_Matrix Translation(float x,float y,float z)
	{
		this->_11=1.0f;	this->_12=0.0f;	this->_13=0.0f;	this->_14=0.0f;
		this->_21=0.0f;	this->_22=1.0f;	this->_23=0.0f;	this->_24=0.0f;
		this->_31=0.0f;	this->_32=0.0f;	this->_33=1.0f;	this->_34=0.0f;
		this->_41=x;	this->_42=y;	this->_43=z;	this->_44=1.0f;
		return *this;
	}


	Pt_Matrix RotateX(float Angle)
	{
		int Index=((pt_int)(Angle*10))%3600;
		Index<0?Index+=3600:0;

		this->_11=1.0f;						this->_12=0;							this->_13=0.0f;								this->_14=0.0f;
		this->_21=0.0f;						this->_22=pt_cosx_angle[Index];			this->_23=pt_sinx_angle[Index];				this->_24=0.0f;
		this->_31=0.0f;						this->_32=-pt_sinx_angle[Index];		this->_33=pt_cosx_angle[Index];				this->_34=0.0f;
		this->_41=0.0f;						this->_42=0.0f;							this->_43=0.0f;								this->_44=1.0f;
		return *this;
	}

	Pt_Matrix RotateY(float Angle)
	{
		int Index=((pt_int)(Angle*10))%3600;
		Index<0?Index+=3600:0;

		this->_11=pt_cosx_angle[Index];		this->_12=0.0f;							this->_13=pt_sinx_angle[Index];			this->_14=0.0f;
		this->_21=0.0f;						this->_22=1.0f;							this->_23=0.0f;							this->_24=0.0f;
		this->_31=-pt_sinx_angle[Index];	this->_32=0.0f;							this->_33=pt_cosx_angle[Index];			this->_34=0.0f;
		this->_41=0.0f;						this->_42=0.0f;							this->_43=0.0f;							this->_44=1.0f;
		return *this;
	}

	Pt_Matrix RotateZ(float Angle)
	{
		int Index=((pt_int)(Angle*10))%3600;
		Index<0?Index+=3600:0;

		this->_11=pt_cosx_angle[Index];		this->_12=pt_sinx_angle[Index];			this->_13=0.0f;			this->_14=0.0f;
		this->_21=-pt_sinx_angle[Index];;	this->_22=pt_cosx_angle[Index];			this->_23=0.0f;			this->_24=0.0f;
	this->_31=0.0f;						this->_32=0.0f;							this->_33=1.0f;			this->_34=0.0f;
		this->_41=0.0f;						this->_42=0.0f;							this->_43=0.0f;			this->_44=1.0f;
		return *this;
	}

	Pt_Matrix Scale(float x,float y,float z)
	{
		this->_11=x;	this->_12=0.0f;	this->_13=0.0f;	this->_14=0.0f;
		this->_21=0.0f;	this->_22=y;	this->_23=0.0f;	this->_24=0.0f;
		this->_31=0.0f;	this->_32=0.0f;	this->_33=z;	this->_34=0.0f;
		this->_41=0.0f;	this->_42=0.0f;	this->_43=0.0f;	this->_44=1.0f;
		return *this;
	}

	pt_bool Inverse()
	{
		return Gauss(this->m,this->m);
	}

	Pt_Matrix Transpose()
	{
		Pt_Matrix mat=*this;
		this->_11=mat._11;	this->_12=mat._21;	this->_13=mat._31;	this->_14=mat._41;
		this->_21=mat._12;	this->_22=mat._22;	this->_23=mat._32;	this->_24=mat._42;
		this->_31=mat._13;	this->_32=mat._23;	this->_33=mat._33;	this->_34=mat._43;
		this->_41=mat._14;	this->_42=mat._24;	this->_43=mat._34;	this->_44=mat._44;
		return mat;

	}
private:
	float ptabs(float x){return x>0?x:-x;}
	bool Gauss(float A[][4], float B[][4])
	{

		int i, j, k;
		float max, temp;
		float t[4][4];                
		for (i = 0; i < 4; i++)        
		{
			for (j = 0; j < 4; j++)
			{
				t[i][j] = A[i][j];
			}
		}
		for (i = 0; i < 4; i++)        
		{
			for (j = 0; j < 4; j++)
			{
				B[i][j] = (i == j) ? (float)1 : 0;
			}
		}
		for (i = 0; i < 4; i++)
		{
			max = t[i][i];
			k = i;
			for (j = i+1; j < 4; j++)
			{
				if (ptabs(t[j][i]) > ptabs(max))
				{
					max = t[j][i];
					k = j;
				}
			}
			if (k != i)
			{
				for (j = 0; j < 4; j++)
				{
					temp = t[i][j];
					t[i][j] = t[k][j];
					t[k][j] = temp;
					temp = B[i][j];
					B[i][j] = B[k][j];
					B[k][j] = temp; 
				}
			}
			if (t[i][i] == 0)
			{
				return PT_FALSE;
			}
			temp = t[i][i];
			for (j = 0; j < 4; j++)
			{
				t[i][j] = t[i][j] / temp;        
				B[i][j] = B[i][j] / temp;        
			}
			for (j = 0; j < 4; j++)       
			{
				if (j != i)             
				{
					temp = t[j][i];
					for (k = 0; k < 4; k++)       
					{
						t[j][k] = t[j][k] - t[i][k]*temp;
						B[j][k] = B[j][k] - B[i][k]*temp;
					}
				}
			}
		}
		return PT_TRUE;
	}
};

	
struct pt_color
{
	pt_color()
	{
		_argb.ucolor=0;
	}
	pt_color(pt_ulong ul)
	{
		_argb.ucolor=ul;
	}



	pt_color(pt_uchar r,pt_uchar g,pt_uchar b)
	{
		_argb.a=0xff;
		_argb.r=r;
		_argb.g=g;
		_argb.b=b;
	}
	pt_color(pt_uchar a,pt_uchar r,pt_uchar g,pt_uchar b)
	{
		_argb.a=a;
		_argb.r=r;
		_argb.g=g;
		_argb.b=b;
	}
	pt_color(float a,float r,float g,float b)
	{
		if (a>1.0)
		{
			a=a-(int)a;
		}
		if (r>1.0)
		{
			r=r-(int)r;
		}
		if (g>1.0)
		{
			g=g-(int)g;
		}
		if (b>1.0)
		{
			b=b-(int)b;
		}

		_argb.a=(pt_uchar)(a*255);
		_argb.r=(pt_uchar)(r*255);
		_argb.g=(pt_uchar)(g*255);
		_argb.b=(pt_uchar)(b*255);
	}
	union 
	{
		struct
		{
		pt_uchar b;
		pt_uchar g;
		pt_uchar r;
		pt_uchar a;
		};
		pt_ulong ucolor;
	}_argb;

	pt_uchar a(){return this->_argb.a;}
	pt_uchar r(){return this->_argb.r;}
	pt_uchar g(){return this->_argb.g;}
	pt_uchar b(){return this->_argb.b;}

	pt_color operator+(pt_color& cl)
	{
		pt_color color;
		color._argb.a=cl.a()+_argb.a;
		color._argb.r=cl.a()+_argb.r;
		color._argb.g=cl.a()+_argb.g;
		color._argb.b=cl.a()+_argb.b;
		return color;
	}

	pt_color operator-(pt_color& cl)
	{
		pt_color color;
		color._argb.a=-cl.a()+_argb.a;
		color._argb.r=-cl.a()+_argb.r;
		color._argb.g=-cl.a()+_argb.g;
		color._argb.b=-cl.a()+_argb.b;
		return color;
	}

	pt_color& operator+=(pt_color& cl)
	{
		this->_argb.a=cl.a()+_argb.a;
		this->_argb.r=cl.a()+_argb.r;
		this->_argb.g=cl.a()+_argb.g;
		this->_argb.b=cl.a()+_argb.b;
		return *this;
	}

	pt_color& operator-=(pt_color& cl)
	{
		this->_argb.a=-cl.a()+_argb.a;
		this->_argb.r=-cl.a()+_argb.r;
		this->_argb.g=-cl.a()+_argb.g;
		this->_argb.b=-cl.a()+_argb.b;
		return *this;
	}

	pt_bool operator==(pt_color& cl)
	{
		return _argb.ucolor==cl._argb.ucolor;
	}


	pt_bool operator!=(pt_color& cl)
	{
		return *this==cl;
	}

	operator pt_ulong() const
	{
		return _argb.ucolor;
	}
};


struct pt_point
{
	pt_float x;
	pt_float y;
	pt_float z;
	pt_point()
	{
		x=0;
		y=0;
		z=0;
	}
	pt_point(pt_float x,pt_float y,pt_float z)
	{
		this->x=x;this->y=y;this->z=z;
	}
	pt_point(pt_float x,pt_float y)
	{
		this->x=x;this->y=y;this->z=0;
	}

	pt_point(pt_float *pfp)
	{
		x=pfp[0];
		y=pfp[1];
		z=pfp[2];
	}


	pt_point operator+(pt_point& cl)
	{
		pt_point point;
		point.x=this->x+cl.x;
		point.y=this->y+cl.y;
		point.z=this->z+cl.z;
		return point;
	}

	pt_point operator-(pt_point& cl)
	{
		pt_point point;
		point.x=this->x-cl.x;
		point.y=this->y-cl.y;
		point.z=this->z-cl.z;
		return point;
	}

	pt_point operator*(pt_float fl)
	{
		pt_point point;
		point.x=this->x*fl;
		point.y=this->y*fl;
		point.z=this->z*fl;
		return point;
	}

	pt_point operator*(pt_matrix mat)
	{
		pt_point point;
		point.x=x*mat._11+y*mat._21+z*mat._31+1*mat._41;
		point.y=x*mat._12+y*mat._22+z*mat._32+1*mat._42;
		point.z=x*mat._13+y*mat._23+z*mat._33+1*mat._43;
		return point;
	}

	pt_point operator/(pt_float fl)
	{
		pt_point point;
		point.x=this->x/fl;
		point.y=this->y/fl;
		point.z=this->z/fl;
		return point;
	}

	pt_point& operator+=(pt_point& cl)
	{
		this->x+=cl.x;
		this->y+=cl.y;
		this->z+=cl.z;
		return *this;
	}

	pt_point& operator-=(pt_point& cl)
	{
		this->x-=cl.x;
		this->y-=cl.y;
		this->z-=cl.z;
		return *this;
	}

	pt_point& operator*=(pt_float fl)
	{
		this->x*=fl;
		this->y*=fl;
		this->z*=fl;
		return *this;
	}

	pt_point& operator*=(pt_matrix mat)
	{
		this->x=x*mat._11+y*mat._12+z*mat._13+1*mat._14;
		this->y=x*mat._21+y*mat._22+z*mat._23+1*mat._24;
		this->z=x*mat._31+y*mat._32+z*mat._33+1*mat._34;
		return *this;
	}
	pt_point& operator/=(pt_float fl)
	{
		this->x/=fl;
		this->y/=fl;
		this->z/=fl;
		return *this;
	}

	pt_float dot(pt_point &vec)
	{
		return x*vec.x+y*vec.y+z*vec.z;
	}
	pt_point cross(pt_point &vec)
	{
		return pt_point(y*vec.z-vec.y*z,z*vec.x-vec.z*x,x*vec.y-vec.x*y);
	}
	pt_float mod()
	{
		return sqrt(x*x+y*y+z*z);
	}
	pt_float square()
	{
		return (x*x+y*y+z*z);
	}
	pt_point unit()
	{
		return *this/mod();
	}
};

typedef pt_point  pt_vector;
 


struct pt_rect
{
	pt_float x,y,width,height;
	pt_rect()
	{
		x=0;y=0;width=0;height=0;
	}
	pt_rect(float x,float y,float width,float height)
	{
		this->x=x;
		this->y=y;
		this->width=width;
		this->height=height;
	}

	pt_rect(pt_point p1,pt_point p2)
	{
		pt_float minx,miny,maxx,maxy;
		if (p1.x<p2.x)
		{
			minx=p1.x;
			maxx=p2.x;
		}
		else
		{
			minx=p2.x;
			maxx=p1.x;
		}

		if (p1.y<p2.y)
		{
			miny=p1.y;
			maxy=p2.y;
		}
		else
		{
			miny=p2.y;
			maxy=p1.y;
		}
		this->x=minx;this->y=miny;
		width=maxx-minx;
		height=maxy-miny;
	}
};



struct pt_vertex
{
	pt_point	Position;
	pt_vector	Normal;
	pt_color	Color;
	pt_float	u,v;
};


enum PT_STRING_FROM_NUMBER
{
	PT_STRING_FROM_NUMBER_TO_DEX	=0,
	PT_STRING_FROM_NUMBER_TO_HEX	=1,
	PT_STRING_FROM_NUMBER_TO_BIN	=2,
};

class pt_string
{
public:
#define PT_STRING_DEFAULT_LENGTH 32
	pt_string()
	{
		buffer=(pt_char *)new pt_char[PT_STRING_DEFAULT_LENGTH];
		bufferlen=PT_STRING_DEFAULT_LENGTH;
		buffer[0]='\0';
	}
	pt_string(const char *str)
	{
		pt_uchar shl=0;
		pt_uint length=strlen(str);
		while ((pt_uint)(1<<++shl)<=length);
		bufferlen=(1<<shl);
		buffer=new pt_char[bufferlen];
		PT_STRCPY(buffer,str);
	}

	pt_string(char *str)
	{
		pt_uchar shl=0;
		pt_uint length=strlen(str);
		while ((pt_uint)(1<<++shl)<=length);
		bufferlen=(1<<shl);
		buffer=(pt_char *)new pt_char[bufferlen];
		PT_STRCPY(buffer,str);
	}

	pt_string::pt_string(const pt_string& ptstr) 
	{
		if (&ptstr!=this)
		{
			bufferlen=ptstr.bufferlen;
			buffer=new pt_char[bufferlen];
			memcpy(buffer,ptstr.buffer,bufferlen);
		}
	}

	pt_bool operator == (const char *str)
	{
		return strcmp(buffer,str)==0;
	}
	
	pt_bool operator == (pt_string &str)
	{
		return strcmp(buffer,str.buffer)==0;
	}

	pt_bool operator != (const char *str)
	{
		return strcmp(buffer,str);
	}
	
	pt_bool operator != (pt_string &str)
	{
		return strcmp(buffer,str.buffer);
	}

	pt_void operator =(const char *str)
	{
		delete [] buffer;
		pt_char shl=0;
		pt_uint length=strlen(str);
		while ((pt_uint)(1<<++shl)<=length);
		bufferlen=(1<<shl);
		buffer=new pt_char[bufferlen];
		PT_STRCPY(buffer,str);
	}

	pt_void operator =(char *str)
	{
		delete [] buffer;
		pt_uchar shl=0;
		pt_uint length=strlen(str);
		while ((pt_uint)(1<<++shl)<=length);
		bufferlen=(1<<shl);
		buffer=new pt_char[bufferlen];
		PT_STRCPY(buffer,str);
	}



	pt_string & operator=(const pt_string ptstr)
	{
		if(this == &ptstr)
			return *this;

		delete [] buffer;
		bufferlen = ptstr.bufferlen;
		buffer = (pt_char *)new pt_char[bufferlen];
		PT_STRCPY(buffer, ptstr.buffer);
		return *this;
	};



	pt_string operator+(pt_string& ptstr)
	{
		pt_char shl=0;
		pt_string  ptstring;
		ptstring.free();
		pt_uint length=this->stringlength()+ptstr.stringlength();
		while ((pt_uint)(1<<++shl)<=length);
		ptstring.bufferlen=(1<<shl);
		ptstring.buffer=(pt_char *)new pt_char[ptstring.bufferlen];

		memcpy(ptstring.buffer,buffer,length);
		PT_STRCAT(ptstring.buffer,ptstr.buffer);

		return ptstring;
	}

	pt_string operator+(const pt_char* str)
	{
		pt_char shl=0;
		pt_string  ptstring;
		ptstring.free();
		pt_uint length=this->stringlength()+strlen(str);
		while ((pt_uint)(1<<++shl)<=length);
		ptstring.bufferlen=(1<<shl);
		ptstring.buffer=(pt_char *)new pt_char[ptstring.bufferlen];
		memcpy(ptstring.buffer,buffer,length);
		PT_STRCAT(ptstring.buffer,str);

		return ptstring;
	}

	pt_string operator+(const pt_char chr)
	{
		pt_char shl=0;
		pt_string  ptstring;
		ptstring.free();
		pt_uint length=this->stringlength()+1;
		while ((pt_uint)(1<<++shl)<=length);
		ptstring.bufferlen=(1<<shl);
		ptstring.buffer=(pt_char *)new pt_char[ptstring.bufferlen];
		memcpy(ptstring.buffer,buffer,stringlength());
		ptstring.buffer[length-1]=chr;
		ptstring.buffer[length]='\0';
		return ptstring;
	}

	pt_string& operator+=(pt_string ptstr)
	{
		pt_char shl=0;
		pt_char *old=buffer;
		pt_uint length=this->stringlength()+ptstr.stringlength();
		if (length<bufferlen)
		{
			PT_STRCAT(buffer,ptstr.buffer);
			return *this;
		}

		while ((pt_uint)(1<<++shl)<=length);
		bufferlen=(1<<shl);
		buffer=(pt_char *)new pt_char[bufferlen];
		PT_STRCPY(buffer,old);
		PT_STRCAT(buffer,ptstr.buffer);
		delete [] old;

		return *this;
	}

	pt_string& operator+=(const pt_char* str)
	{
		pt_uchar shl=0;
		pt_char *old=buffer;
		pt_uint length=this->stringlength()+strlen(str);
		if (length<bufferlen)
		{
			PT_STRCAT(buffer,str);
			return *this;
		}

		while ((pt_uint)(1<<++shl)<=length);
		bufferlen=(1<<shl);
		buffer=(pt_char *)new pt_char[bufferlen];
		PT_STRCPY(buffer,old);
		PT_STRCAT(buffer,str);
		delete [] old;

		return *this;
	}

	pt_string& operator+=(const pt_char chr)
	{
		pt_uchar shl=0;
		pt_char *old=buffer;
		pt_uint length=this->stringlength()+1;
		if (length<bufferlen)
		{
			buffer[length-1]=chr;
			buffer[length]='\0';
			return *this;
		}

		while ((pt_uint)(1<<++shl)<=length);
		bufferlen=(1<<shl);
		buffer=(pt_char *)new pt_char[bufferlen];
		PT_STRCPY(buffer,old);
		buffer[length-1]=chr;
		buffer[length]='\0';
		delete [] old;
		return *this;
	}



	operator pt_char *()
	{
		return buffer;
	}

	pt_int toInt()
	{
		if (buffer[0]=='0'&&(buffer[1]=='x'||buffer[1]=='X'))
		{
			return (pt_int)hex_to_decimal(buffer+2);
		}
		else
		return atoi(buffer);
	}

	pt_float toFloat()
	{
		return (pt_float)atof(buffer);
	}
	


	pt_string &Number(pt_int number,PT_STRING_FROM_NUMBER numType=PT_STRING_FROM_NUMBER_TO_DEX)
	{
		pt_string str;
		char bufNumber[32+8]={0};
		switch(numType)
		{
		case  PT_STRING_FROM_NUMBER_TO_DEX:
			PT_ITOA(number,bufNumber,10);
			break;
		case PT_STRING_FROM_NUMBER_TO_HEX:
			PT_ITOA(number,bufNumber,16);
			break;
		case PT_STRING_FROM_NUMBER_TO_BIN:
			PT_ITOA(number,bufNumber,16);
			break;
		}
		
		*this=bufNumber;
		return *this;
	}

	pt_int stringlength()
	{
		return strlen(buffer);
	}

	pt_void clear()
	{
		memset(buffer,0,bufferlen);
	}

	pt_string toUpperCase()
	{
		strupr(buffer);
		return *this;
	}
	
	pt_string toLowerCase()
	{
		strlwr(buffer);
		return *this;
	}
	

	~pt_string()
	{
		free();
	}
	void free()
	{
		if(buffer)
		{
			delete [] buffer;
			buffer=NULL;
		}
		bufferlen=0;
	}

pt_uint   bufferlen;
pt_char *buffer;

private:
	pt_uint hex_to_decimal(char* hex_str)   
	{  
		char ch;   
		unsigned int iret=0;  
		while(ch=*hex_str++)   
		{   
			iret=(iret<<4)|hex_to_dex_table[ch];  
		}   
		return iret;   
	} 
};

class pt_buffer
{
public:
	pt_buffer()
	{
		m_buffer=NULL;
		m_size=0;
		m_allocSize=0;
	}

	pt_buffer(pt_uint size)
	{
		m_buffer=new pt_byte[size];
		m_size=size;
		m_allocSize=m_size;
	}

	~pt_buffer()
	{
		if (m_buffer)
		{
			delete [] m_buffer;
			m_size=0;
			m_allocSize=0;
		}
	}

	pt_buffer::pt_buffer(const pt_buffer& ptbuf)
	{
		m_buffer=new pt_byte[ptbuf.m_allocSize];
		m_size=ptbuf.m_size;
		m_allocSize=ptbuf.m_allocSize;
		memcpy(m_buffer,ptbuf.m_buffer,m_size);
	}


	operator pt_byte *()
	{
		return m_buffer;
	}

	pt_buffer & operator=(const pt_buffer &ptbuf)
	{
		if(this == &ptbuf)
			return *this;

		if(m_buffer!=NULL)
			delete [] m_buffer;

		m_buffer=new pt_byte[ptbuf.m_allocSize];
		m_size=ptbuf.m_size;
		m_allocSize=ptbuf.m_allocSize;
		memcpy(m_buffer,ptbuf.m_buffer,m_size);
		return *this;
	};

	pt_buffer& operator+=(const pt_buffer& ptbuf)
	{
		memcat(ptbuf.m_buffer,ptbuf.m_size);
		return *this;
	}

	pt_buffer operator+(const pt_buffer& ptbuf)
	{
		pt_buffer buf;
		buf.memcat(m_buffer,m_size);
		buf.memcat(ptbuf.m_buffer,ptbuf.m_size);
		return buf;
	}


	pt_void resize(pt_uint size)
	{
		if(m_buffer!=NULL)
			delete [] m_buffer;

			m_buffer=new pt_byte[size];
			this->m_size=size;
			m_allocSize=size;
	}

	pt_void free()
	{
		if(m_buffer!=NULL)
			delete [] m_buffer;
		m_buffer=NULL;
		m_size=0;
		m_allocSize=0;
	}

	pt_void memcat(pt_byte *buffer,pt_uint size)
	{
		
		if (m_buffer==NULL)
		{
			m_buffer=new pt_byte[size];
			memcpy(m_buffer,buffer,size);
			m_size=size;
			m_allocSize=size;
		}
		else
		{

			if (m_size+size<=m_allocSize)
			{
				memcpy(m_buffer+m_size,buffer,size);
				m_size=m_size+size;
			}
			else
			{
			pt_uint newsize=size+m_size;
			pt_uint shl=0;
			while ((pt_uint)(1<<++shl)<=newsize);
			newsize=(1<<shl);
			pt_byte *oldbuf=m_buffer;
			m_buffer=new pt_byte[newsize];  
			memcpy(m_buffer,oldbuf,m_size);
			memcpy(m_buffer+m_size,buffer,size);
			delete [] oldbuf;
			m_size=m_size+size;
			m_allocSize=newsize;
			}
		}
	}

	pt_uint write(pt_byte *buf,pt_uint offset,pt_uint size)
	{
		pt_uint writeSize=0;
		if (offset+size>m_size)
		{
		writeSize=m_size-offset;
		}else
		{
		writeSize=size;
		}
		if (writeSize==0)
		{
		return 0;
		}
		memcpy(m_buffer+offset,buf,writeSize);
		return writeSize;
	}

	pt_uint read(pt_byte *buf,pt_uint offset,pt_uint size)
	{
		pt_uint readSize=0;
		if (offset+size>m_size)
		{
		readSize=m_size-offset;
		}
		else
		{
		readSize=size;
		}
		if (readSize==0)
		{
		return 0;
		}
		memcpy(buf,m_buffer+offset,readSize);
		return readSize;
	}
	pt_byte *m_buffer;
	pt_uint m_size;
	private:
	pt_uint m_allocSize;
};