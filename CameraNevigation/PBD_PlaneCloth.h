#ifndef __PBD_PLANE_CLOTH_H__
#define __PBD_PLANE_CLOTH_H__

#pragma once
#include "Vec3.h"
#include <vector>

using namespace std;

class PBD_Spring
{
public:
	int		m_Particle[2];
	double	m_RestLength;
	double	m_Ks;
	double	m_Kd;
};

class PBD_PlaneCloth
{
public:
	int						m_Res[2]; // width and height
	vector<double>			m_InvMass;
	vector<Vec3<double>>	m_Pos;
	vector<Vec3<double>>	m_NewPos;
	vector<Vec3<double>>	m_Vel;
public:
	vector<PBD_Spring>		m_Springs;
public:
	PBD_PlaneCloth();
	PBD_PlaneCloth(int width, int height);
	~PBD_PlaneCloth();
public:
	inline Vec3<double>	GetPos(int i, int j) { return m_Pos[j * m_Res[0] + i]; }
	inline void AddVel(int i, int j, Vec3<double> v) {m_Vel[j * m_Res[0] + i] += v; }
public:
	void	Init(void);
	void	Draw(void);
	void	DrawOutline(void);
	void	Simulation(double dt);
	void	ballCollision(const Vec3<double> center, const double radius);
	
	
};



#endif
