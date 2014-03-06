#include "Utils.h"



namespace Utils {

	bool isOpenGLError() {
		bool isError = false;
		GLenum errCode;
		const GLubyte *errString;

		while ((errCode = glGetError()) != GL_NO_ERROR) {		
			isError = true;
			errString = gluErrorString(errCode);
			std::cerr << "OpenGL ERROR [" << errString << "]." << std::endl;
		}
		return isError;
	}
	

	void checkOpenGLError(std::string error) {
		int maxLength;
		char result[128];

		if(isOpenGLError()) {
			std::cerr << error << std::endl;

			glGetShaderInfoLog(ProgramShader::getInstance()->getUId("VertexShader"), sizeof(result), &maxLength, result);
			if(maxLength > 11)
				std::cerr << "Error Vertex Shader: " << result;

			glGetShaderInfoLog(ProgramShader::getInstance()->getUId("FragmentShader"), sizeof(result), &maxLength, result);
			if(maxLength > 11)
				std::cerr << "Error Fragment Shader: " << result;

			glGetProgramInfoLog(ProgramShader::getInstance()->getUId("Program"), sizeof(result), &maxLength, result);
			if(maxLength > 11)
				std::cerr << "Error Program: " << result << std::endl;

			exit(EXIT_FAILURE);
		}
	}

	
	GLchar* readFile(char* path){
		std::string line, result;
		std::ifstream myfile(path, std::ios::in);

		char* buffer;
		int size;

		if (!myfile){
			std::cerr << "Cannot open " << path << std::endl;
			exit(1);
		}

		if (myfile.is_open()) {
			while (getline(myfile, line)) {
				result += line + "\n";
			}
			myfile.close();
		}

		size = result.size();
		buffer = new char[size];
		memcpy(buffer, result.c_str(), size);
		buffer[size-1] = '\0';
	
		return buffer;
	}


	void loadObj(char* filename, std::vector<unsigned int> &indices, std::vector<glm::vec3> &out_vertices, std::vector<glm::vec2> &out_uvs, std::vector<glm::vec3> &out_normals){
		std::vector<glm::vec3> vertices;
		std::vector<glm::vec2> uvs;
		std::vector<glm::vec3> normals;
		std::vector<unsigned int> uvIndices;
		std::vector<unsigned int> normalIndices;

		std::ifstream in(filename, std::ios::in);
		if(!in){ 
			std::cerr << "Cannot open " << filename << std::endl; 
			exit(1); 
		}
 
		std::string line;
		while (getline(in, line)) {
			if (line.substr(0,2) == "v "){
				std::istringstream s(line.substr(2));
				glm::vec3 v; 
				s >> v.x >> v.y >> v.z;
				vertices.push_back(v);
			}
			else if (line.substr(0,3) == "vt "){
				std::istringstream s(line.substr(3));
				glm::vec2 v;
				s >> v.x >> v.y;
				uvs.push_back(v);
			}
			else if (line.substr(0,3) == "vn "){
				std::istringstream s(line.substr(3));
				glm::vec3 v; 
				s >> v.x >> v.y >> v.z;
				normals.push_back(v);
			}
			else if (line.substr(0,2) == "f ") {
				std::istringstream s(line.substr(2));
				std::string item, face;
				int i = 0;
				while(getline(s, face, ' ')){
					std::istringstream s1(face.substr(0));
					while(getline(s1, item, '/')){
						if(item.size() && i == 0 && stoi(item) > 0 && stoi(item) <= vertices.size())
							indices.push_back(stoi(item)-1);
						else if(item.size() && i == 1 && stoi(item) > 0 && stoi(item) <= uvs.size())
							uvIndices.push_back(stoi(item)-1);
						else if(item.size() && i == 2 && stoi(item) > 0 && stoi(item) <= normals.size())
							normalIndices.push_back(stoi(item)-1);
						else{
							std::cerr << "Invalid index on " << filename << std::endl; 
							exit(1); 
						}
						i = ++i % 3; 
					}
				}
			}
		}

		for(unsigned int i = 0; i < indices.size(); i++){
			unsigned int vertexIndex = indices[i];
			unsigned int uvIndex = uvIndices[i];
			unsigned int normalIndex = normalIndices[i];
		
			glm::vec3 vertex = vertices[vertexIndex];
			glm::vec2 uv = uvs[uvIndex];
			glm::vec3 normal = normals[normalIndex];
		
			out_vertices.push_back(vertex);
			out_uvs.push_back(uv);
			out_normals.push_back(normal);
		}
	}


	void loadMaterial(char* filename, glm::vec3 &ambient, glm::vec3 &diffuse, glm::vec3 &specular, float &shininess){
		std::ifstream in(filename, std::ios::in);
		if(!in){ 
			std::cerr << "Cannot open " << filename << std::endl; 
			exit(1); 
		}

		std::string line;
		while (getline(in, line)) {
			if (line.substr(0,3) == "Ka "){
				std::istringstream s(line.substr(3));
				s >> ambient.x >> ambient.y >> ambient.z;
			}
			else if (line.substr(0,3) == "Kd "){
				std::istringstream s(line.substr(3));
				s >> diffuse.x >> diffuse.y >> diffuse.z;
			}
			else if (line.substr(0,3) == "Ks "){
				std::istringstream s(line.substr(3));
				s >> specular.x >> specular.y >> specular.z;
			}
			else if (line.substr(0,3) == "Ns "){
				std::istringstream s(line.substr(3));
				s >> shininess;
			}

		}
	}


	tImageTGA *loadTexture(char *strfilename){
		tImageTGA *pImgData;
		FILE *pFile;
		WORD width = 0;
		WORD height = 0;
		byte length = 0;
		byte imgType = 0;
		byte bits = 0;
		int channels = 0;
		int stride = 0;
		int i = 0;

		if ((pFile = fopen(strfilename, "rb")) == NULL){
			std::cout << "Error loading tga file" << std::endl;
			return NULL;
		}

		pImgData = (tImageTGA*)malloc(sizeof(tImageTGA));
		fread(&length, sizeof(byte), 1, pFile);
		fseek(pFile, 1, SEEK_CUR);
		fread(&imgType, sizeof(byte), 1, pFile);
		fseek(pFile, 9, SEEK_CUR);

		fread(&width, sizeof(WORD), 1, pFile);
		fread(&height, sizeof(WORD), 1, pFile);
		fread(&bits, sizeof(byte), 1, pFile);

		fseek(pFile, length + 1, SEEK_CUR);

		if (imgType != TGA_RLE){
			if (bits == 24 || bits == 32){
				channels = bits / 8;
				stride = channels * width;
				pImgData->data = new unsigned char[stride * height];

				for (int y = 0; y < height; y++){
					unsigned char *pLine = &(pImgData->data[stride * y]);
					fread(pLine, stride, 1, pFile);
					for (i = 0; i < stride; i += channels){
						int temp = pLine[i];
						pLine[i] = pLine[i + 2];
						pLine[i + 2] = temp;
					}
				}
			}
			else if (bits == 16)
			{
				unsigned short pixels = 0;
				int r = 0, g = 0, b = 0;

				channels = 3;
				stride = channels * width;
				pImgData->data = new unsigned char[stride * height];

				for (int i = 0; i < width*height; i++){
					fread(&pixels, sizeof(unsigned short), 1, pFile);

					b = (pixels & 0x1f) << 3;
					g = ((pixels >> 5) & 0x1f) << 3;
					r = ((pixels >> 10) & 0x1f) << 3;

					pImgData->data[i * 3 + 0] = r;
					pImgData->data[i * 3 + 1] = g;
					pImgData->data[i * 3 + 2] = b;
				}
			}
			else
				return NULL;
		}
		else{

			byte rleID = 0;
			int colorsRead = 0;
			channels = bits / 8;
			stride = channels * width;

			pImgData->data = new unsigned char[stride * height];
			byte *pColors = new byte[channels];

			while (i < width*height){

				fread(&rleID, sizeof(byte), 1, pFile);
				if (rleID < 128){
					rleID++;

					while (rleID){
						fread(pColors, sizeof(byte)* channels, 1, pFile);

						pImgData->data[colorsRead + 0] = pColors[2];
						pImgData->data[colorsRead + 1] = pColors[1];
						pImgData->data[colorsRead + 2] = pColors[0];

						if (bits == 32)	pImgData->data[colorsRead + 3] = pColors[3];

						i++;
						rleID--;
						colorsRead += channels;
					}
				}

				else{
					rleID -= 127;

					fread(pColors, sizeof(byte)* channels, 1, pFile);

					while (rleID)
					{
						pImgData->data[colorsRead + 0] = pColors[2];
						pImgData->data[colorsRead + 1] = pColors[1];
						pImgData->data[colorsRead + 2] = pColors[0];

						if (bits == 32)	pImgData->data[colorsRead + 3] = pColors[3];

						i++;
						rleID--;
						colorsRead += channels;
					}
				}
			}
			delete[] pColors;
		}

		fclose(pFile);
		pImgData->channels = channels;
		pImgData->size_x = width;
		pImgData->size_y = height;

		return pImgData;
	}
}
