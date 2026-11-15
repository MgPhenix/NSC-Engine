#include "Shape.h"

#include <ToolsBox/ToolsBox.h>
#include <SDL3/SDL.h>
#include <iostream>
#include <vector>
#include "GPU.h"
#include "stb_image.h"

#pragma region Shape

void Shape::CreateVertexBuffer()
{
	GPU& gpu = GPU::GetInstance();

	SDL_GPUBufferCreateInfo buffer_info = {};
	buffer_info.usage = SDL_GPU_BUFFERUSAGE_VERTEX;
	buffer_info.size = sizeof(float) * 9 * m_num_vertices;

	mp_vertex_buffer = SDL_CreateGPUBuffer(gpu.GetDevice(), &buffer_info);

	if (mp_vertex_buffer == NULL)
		std::cout << "Can't create vertex buffer" << std::endl;
}

void Shape::CreateIndexBuffer()
{
	GPU& gpu = GPU::GetInstance();

	SDL_GPUBufferCreateInfo buffer_info = {};
	buffer_info.usage = SDL_GPU_BUFFERUSAGE_INDEX; //Permet de definir les sommets et de choisir l'ordre dans lequel dessiner les triangles
	buffer_info.size = sizeof(Uint16) * m_num_indices;

	mp_index_buffer = SDL_CreateGPUBuffer(gpu.GetDevice(), &buffer_info);

	if (mp_index_buffer == NULL)
		std::cout << "Can't create index buffer" << std::endl;
}

bool Shape::CreateTexture(const char* path)
{
	SDL_GPUDevice* device = GPU::GetInstance().GetDevice();

	int width, height, channels;
	unsigned char* pixels = stbi_load(path, &width, &height, &channels, STBI_rgb_alpha);

	if (pixels == NULL)
	{
		std::cout << "Can't load image" << std::endl;
		return false;
	}

	SDL_GPUTextureCreateInfo texture_info = {};
	texture_info.type = SDL_GPU_TEXTURETYPE_2D;
	texture_info.format = SDL_GPU_TEXTUREFORMAT_R8G8B8A8_UNORM;
	texture_info.usage = SDL_GPU_TEXTUREUSAGE_SAMPLER;
	texture_info.height = height;
	texture_info.width = width;
	texture_info.layer_count_or_depth = 1;
	texture_info.num_levels = 1;

	mp_texture = SDL_CreateGPUTexture(device, &texture_info);

	if (mp_texture == NULL)
	{
		std::cout << "Can't create gpu texture : " << SDL_GetError() << std::endl;
		return false;
	}

	SDL_GPUTransferBufferCreateInfo transfer_info = {};
	transfer_info.usage = SDL_GPU_TRANSFERBUFFERUSAGE_UPLOAD;
	transfer_info.size = width * height * 4;

	SDL_GPUTransferBuffer* transfer_buffer = SDL_CreateGPUTransferBuffer(device, &transfer_info);

	void* map = SDL_MapGPUTransferBuffer(device, transfer_buffer, false);
	memcpy(map, pixels, width * height * 4);
	SDL_UnmapGPUTransferBuffer(device, transfer_buffer);

	SDL_GPUCommandBuffer* cmd = SDL_AcquireGPUCommandBuffer(device);
	SDL_GPUCopyPass* copy_pass = SDL_BeginGPUCopyPass(cmd);

	SDL_GPUTextureTransferInfo source = {};
	source.transfer_buffer = transfer_buffer;
	source.offset = 0;

	SDL_GPUTextureRegion destination = {};
	destination.texture = mp_texture;
	destination.w = width;
	destination.h = height;
	destination.d = 1;

	SDL_UploadToGPUTexture(copy_pass, &source, &destination, false);

	SDL_EndGPUCopyPass(copy_pass);
	SDL_SubmitGPUCommandBuffer(cmd);
	SDL_ReleaseGPUTransferBuffer(device, transfer_buffer);

	stbi_image_free(pixels);

	return true;
}

void Shape::UploadVertexData(std::vector<Vertice> vertices, size_t vertices_size)
{
	m_vertices = vertices;

	SDL_GPUDevice* device = GPU::GetInstance().GetDevice();

	SDL_GPUTransferBufferCreateInfo transfer_info = {};
	transfer_info.usage = SDL_GPU_TRANSFERBUFFERUSAGE_UPLOAD;
	transfer_info.size = sizeof(float) * 9 * m_num_vertices;

	SDL_GPUTransferBuffer* transfer_buffer = SDL_CreateGPUTransferBuffer(device, &transfer_info);

	void* map = SDL_MapGPUTransferBuffer(device, transfer_buffer, false);
	memcpy(map, vertices.data(), vertices_size);
	SDL_UnmapGPUTransferBuffer(device, transfer_buffer);

	SDL_GPUCommandBuffer* cmd = SDL_AcquireGPUCommandBuffer(device);
	SDL_GPUCopyPass* copy_pass = SDL_BeginGPUCopyPass(cmd);

	SDL_GPUTransferBufferLocation source = {};
	source.transfer_buffer = transfer_buffer;
	source.offset = 0;

	SDL_GPUBufferRegion destination = {};
	destination.buffer = mp_vertex_buffer;
	destination.size = static_cast<Uint32>(vertices_size);
	destination.offset = 0;

	SDL_UploadToGPUBuffer(copy_pass, &source, &destination, false);

	SDL_EndGPUCopyPass(copy_pass);
	SDL_SubmitGPUCommandBuffer(cmd);
	SDL_ReleaseGPUTransferBuffer(device, transfer_buffer);
}

void Shape::UploadIndexData(std::vector<Uint16> indices, size_t indices_size)
{
	m_indices = indices;

	SDL_GPUDevice* device = GPU::GetInstance().GetDevice();

	SDL_GPUTransferBufferCreateInfo transfer_info = {};
	transfer_info.usage = SDL_GPU_TRANSFERBUFFERUSAGE_UPLOAD;
	transfer_info.size = sizeof(Uint16) * m_num_indices;

	SDL_GPUTransferBuffer* transfer_buffer = SDL_CreateGPUTransferBuffer(device, &transfer_info);

	void* map = SDL_MapGPUTransferBuffer(device, transfer_buffer, false);
	memcpy(map, indices.data(), indices_size);
	SDL_UnmapGPUTransferBuffer(device, transfer_buffer);

	SDL_GPUCommandBuffer* cmd = SDL_AcquireGPUCommandBuffer(device);
	SDL_GPUCopyPass* copy_pass = SDL_BeginGPUCopyPass(cmd);

	SDL_GPUTransferBufferLocation source = {};
	source.transfer_buffer = transfer_buffer;
	source.offset = 0;

	SDL_GPUBufferRegion destination = {};
	destination.buffer = mp_index_buffer;
	destination.size = static_cast<Uint32>(indices_size);
	destination.offset = 0;

	SDL_UploadToGPUBuffer(copy_pass, &source, &destination, false);

	SDL_EndGPUCopyPass(copy_pass);
	SDL_SubmitGPUCommandBuffer(cmd);
	SDL_ReleaseGPUTransferBuffer(device, transfer_buffer);
}

void Shape::InitBuffers(std::vector<Vertice> vertices, std::vector<Uint16> indices)
{
	//m_indices  = indices;
	m_num_vertices = static_cast<int>(vertices.size());
	m_num_indices  = static_cast<int>(indices.size());

	CreateVertexBuffer();
	CreateIndexBuffer();

	UploadVertexData(vertices, sizeof(Vertice) * vertices.size());
	UploadIndexData(indices, sizeof(Uint16) * indices.size());
}

void Shape::ReUploadVertexBuffer(std::vector<Vertice> vertices, size_t vertices_size)
{
	SDL_ReleaseGPUBuffer(GPU::GetInstance().GetDevice(), mp_vertex_buffer);
	mp_vertex_buffer = NULL;

	CreateVertexBuffer();
	UploadVertexData(vertices, vertices_size);
}

void Shape::SetTexture(const char* path, bool is_pixel_art)
{	
	if (!CreateTexture(path))
	{
		std::cout << "Couldn't create texture " << std::endl;
		return;
	}

	m_has_texture = true;
	m_is_pixel_art = is_pixel_art;
}

bool Shape::HasTexture()
{
	return m_has_texture;
}

bool Shape::IsPixelArt()
{
	return m_is_pixel_art;
}

Shape::Shape(std::vector<Vector3f> posList, UColor color, std::vector<Uint16> indices) :
	Shape(posList, ConvertColor(color), indices)
{}

Shape::Shape(std::vector<Vector3f> posList, FColor color, std::vector<Uint16> indices)
{
	m_type = Shapes::Custom;

	std::vector<Vertice> vertices;
	for (int i = 0; i < posList.size(); i++)
	{
		vertices.push_back(Vertice(posList[i], color, {0.f, 0.f}));
	}

	InitBuffers(vertices, indices);
}

DrawData Shape::GetDrawData()
{
	DrawData data;
	data.index_buffer = mp_index_buffer;
	data.vertex_buffer = mp_vertex_buffer;
	data.num_indices = m_num_indices;
	data.num_vertices = m_num_vertices;
	data.texture = mp_texture;

	return data;
}

Shape::Shapes Shape::GetShape()
{
	return m_type;
}

void Shape::Resize(float newSizeW, float newSizeH)
{
	//TODO 
	// Go fuck yourself
	std::cout << "You can't\n\nGood Luck" << std::endl;
}

void Shape::ResizePercent(float percentW, float percentH)
{
	//TODO 
	// Go fuck yourself
	std::cout << "You can't\n\nGood Luck" << std::endl;
}

#pragma endregion

#pragma region Triangle

Triangle::Triangle(float width, float height, UColor color) :
	Triangle(width, height, ConvertColor(color))
{}

Triangle::Triangle(float width, float height, FColor color)
{
	m_type = Shapes::Triangle;

	std::vector<Vertice> vertices = {
		Vertice({ 0.f, -height / 2.f,		 0.f }, color,{ 0.f, 0.f }),
		Vertice({-width / 2.f, height / 2.f, 0.f }, color,{ 1.f, 0.f }),
		Vertice({ width / 2.f, height / 2.f, 0.f }, color,{ 0.f, 1.f })
	};

	std::vector<Uint16> indices = {
		0, 1, 2
	};

	m_width = width;
	m_height = height;
	InitBuffers(vertices, indices);
}

void Triangle::Resize(float newSizeW, float newSizeH)
{
	std::vector<Vertice> new_vertices = {
		Vertice({ 0.f,			 -newSizeH / 2.f, 0.f }, m_vertices[0].color, m_vertices[0].uv),
		Vertice({-newSizeW / 2.f, newSizeH / 2.f, 0.f }, m_vertices[1].color, m_vertices[0].uv),
		Vertice({ newSizeW / 2.f, newSizeH / 2.f, 0.f }, m_vertices[2].color, m_vertices[0].uv)
	};
	
	m_vertices = new_vertices;
	m_width = newSizeW;
	m_height = newSizeH;

	ReUploadVertexBuffer(new_vertices, sizeof(Vertice) * 3);
}

void Triangle::ResizePercent(float percentW, float percentH)
{
	Resize(m_width * percentW, m_height * percentH);
}

float Triangle::GetWidth()
{
	return m_width;
}

float Triangle::GetHeight()
{
	return m_height;
}

#pragma endregion

#pragma region Rectangle

Rectangle::Rectangle(float width, float height, UColor color) :
	Rectangle(width, height, ConvertColor(color))
{}

Rectangle::Rectangle(float width, float height, FColor color)
{
	m_type = Shapes::Rectangle;

	std::vector<Vertice> vertices = {
		Vertice({ -width / 2.f, -height / 2.f, 0.f}, color, { 0.f, 0.f }),
		Vertice({  width / 2.f, -height / 2.f, 0.f}, color, { 1.f, 0.f }),
		Vertice({  width / 2.f,  height / 2.f, 0.f}, color, { 1.f, 1.f }),
		Vertice({ -width / 2.f,  height / 2.f, 0.f}, color, { 0.f, 1.f }),
	};

	std::vector<Uint16> indices = {
		0, 1, 2,
		2, 3, 0
	};

	m_width = width;
	m_height = height;
	InitBuffers(vertices, indices);
}

void Rectangle::Resize(float newSizeW, float newSizeH)
{
	std::vector<Vertice> new_vertices = {
		Vertice({ -newSizeW / 2.f, -newSizeH / 2.f, 0.f}, m_vertices[0].color, m_vertices[0].uv),
		Vertice({  newSizeW / 2.f, -newSizeH / 2.f, 0.f}, m_vertices[1].color, m_vertices[1].uv),
		Vertice({  newSizeW / 2.f,  newSizeH / 2.f, 0.f}, m_vertices[2].color, m_vertices[2].uv),
		Vertice({ -newSizeW / 2.f,  newSizeH / 2.f, 0.f}, m_vertices[3].color, m_vertices[3].uv),
	};

	m_vertices = new_vertices;
	m_width = newSizeW;
	m_height = newSizeH;

	ReUploadVertexBuffer(new_vertices, sizeof(Vertice) * 4);
}

void Rectangle::ResizePercent(float percentW, float percentH)
{
	Resize(m_width * percentW, m_height * percentH);
}

float Rectangle::GetWidth()
{
	return m_width;
}

float Rectangle::GetHeight()
{
	return m_height;
}

#pragma endregion

#pragma region Circle

Circle::Circle(float radius, int slices, FColor color, bool filled)
{
	std::vector<Vertice> vertices;
	vertices.push_back(Vertice({ 0.f, 0.f, 0.f }, color, {0.5f, 0.5f}));

	std::vector<Uint16> indices;

	for (int i = 0; i < slices; i++)
	{
		float angle = (2.f * PI * i) / slices;

		float x = radius * std::cos(angle);
		float y = radius * std::sin(angle);

		float u = x / (2 * radius) + 0.5f;
		float v = y / (2 * radius) + 0.5f;;

		vertices.push_back(Vertice({ x, y, 0.f}, color, {}));

		if (filled)
		{
			indices.push_back(0);
			indices.push_back(i + 1);
			indices.push_back((i + 1) % slices + 1);
		}
		else
		{
			indices.push_back(i + 1);
			indices.push_back((i + 1) % slices + 1);
		}
	}

	m_type = Shapes::Circle;
	m_radius = radius;
	m_slices = slices;

	InitBuffers(vertices, indices);
}

Circle::Circle(float radius, int slices, UColor color, bool filled) :
	Circle(radius, slices, ConvertColor(color), filled)
{}

void Circle::Resize(float radius)
{
	FColor color = m_vertices[0].color;

	std::vector<Vertice> vertices;
	vertices.push_back(Vertice({ 0.f, 0.f, 0.f }, color, { 0.5f, 0.5f }));

	for (int i = 0; i < m_slices; i++)
	{
		float angle = (2.f * PI * i) / m_slices;

		float x = radius * std::cos(angle);
		float y = radius * std::sin(angle);

		float u = x / (2 * radius) + 0.5f;
		float v = y / (2 * radius) + 0.5f;;

		vertices.push_back(Vertice({ x, y, 0.f }, color, {}));
	}

	m_radius   = radius;
	m_vertices = vertices;
	
	ReUploadVertexBuffer(vertices, sizeof(Vertice) * vertices.size());
}

void Circle::ResizePercent(float percent)
{
	Resize(m_radius * percent);
}

float Circle::GetRadius()
{
	return m_radius;
}

#pragma endregion