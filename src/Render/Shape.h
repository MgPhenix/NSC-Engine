#pragma once
#include "Color.h"
#include "Vertice.h"
#include <vector>

struct SDL_GPUBuffer;
struct SDL_GPUTexture;
typedef unsigned short Uint16;


struct DrawData
{
	SDL_GPUBuffer* vertex_buffer = nullptr;
	SDL_GPUBuffer* index_buffer = nullptr;

	SDL_GPUTexture* texture = nullptr;

	int num_vertices = 0;
	int num_indices = 0;
};



class Shape
{
public:
		enum Shapes
		{
			Triangle,
			Rectangle,
			Circle,

			Custom,
			None
		};

protected:

	SDL_GPUBuffer* mp_vertex_buffer = nullptr;
	SDL_GPUBuffer* mp_index_buffer  = nullptr;

	SDL_GPUTexture* mp_texture = nullptr;

	int m_num_vertices = 0;
	int m_num_indices  = 0;

	Shapes m_type = Shapes::None;

	bool m_has_texture  = false;
	bool m_is_pixel_art = false;

	std::vector<Vertice> m_vertices;
	std::vector<Uint16>  m_indices;

protected:


	void CreateVertexBuffer();

	void CreateIndexBuffer();

	bool CreateTexture(const char* path);


	void UploadVertexData(std::vector<Vertice> vertices, size_t vertices_size);

	void UploadIndexData(std::vector<Uint16> indices, size_t indices_size);


	void InitBuffers(std::vector<Vertice> vertices, std::vector<Uint16> indices);

	void ReUploadVertexBuffer(std::vector<Vertice> vertices, size_t vertices_size);

	Shape() = default;

public:

	void SetTexture(const char* path, bool is_pixel_art = false);
	bool HasTexture();
	bool IsPixelArt();
	
	Shape(std::vector<Vector3f> posList, UColor color, std::vector<Uint16> indices);
	Shape(std::vector<Vector3f> posList, FColor color, std::vector<Uint16> indices);

	DrawData GetDrawData();
	Shapes	 GetShape();

	virtual void Resize		  (float newSizeW, float newSizeH);
	virtual void ResizePercent(float percentW = 1.f, float percentH = 1.f);
};


class Triangle : public Shape
{
private:

	float m_width  = 0.f;
	float m_height = 0.f;

public:

	Triangle(float width, float height, UColor color);
	Triangle(float width, float height, FColor color);

	void Resize		  (float newSizeW, float newSizeH) override;
	void ResizePercent(float percentW = 1.f, float percentH = 1.f) override;

	float GetWidth();
	float GetHeight();
};

class Rectangle : public Shape
{
private:

	float m_width  = 0.f;
	float m_height = 0.f;

public:

	Rectangle(float width, float height, UColor color);
	Rectangle(float width, float height, FColor color);

	void Resize		  (float newSizeW, float newSizeH) override;
	void ResizePercent(float percentW = 1.f, float percentH = 1.f) override;

	float GetWidth();
	float GetHeight();
};

class Circle : public Shape
{
private:

	float m_radius = 0.f;
	int   m_slices = 0;

public:

	Circle(float radius, int slices, FColor color, bool filled = true);
	Circle(float radius, int slices, UColor color, bool filled = true);

	void Resize		  (float radius);
	void ResizePercent(float percent = 1.f);

	float GetRadius();
};