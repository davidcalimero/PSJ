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

		if (isOpenGLError()) {
			std::cerr << error << std::endl;

			glGetShaderInfoLog(ProgramShader::getInstance()->getUId("VertexShader"), sizeof(result), &maxLength, result);
			if (maxLength > 11)
				std::cerr << "Error Vertex Shader: " << result;

			glGetShaderInfoLog(ProgramShader::getInstance()->getUId("FragmentShader"), sizeof(result), &maxLength, result);
			if (maxLength > 11)
				std::cerr << "Error Fragment Shader: " << result;

			glGetProgramInfoLog(ProgramShader::getInstance()->getUId("Program"), sizeof(result), &maxLength, result);
			if (maxLength > 11)
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
		buffer[size - 1] = '\0';

		return buffer;
	}


	void loadObj(char* filename, std::vector<unsigned int> &indices, std::vector<glm::vec3> &out_vertices, std::vector<glm::vec2> &out_uvs, std::vector<glm::vec3> &out_normals){
		std::vector<glm::vec3> vertices;
		std::vector<glm::vec2> uvs;
		std::vector<glm::vec3> normals;
		std::vector<unsigned int> uvIndices;
		std::vector<unsigned int> normalIndices;

		std::ifstream in(filename, std::ios::in);
		if (!in){
			std::cerr << "Cannot open " << filename << std::endl;
			exit(1);
		}

		std::string line;
		while (getline(in, line)) {
			if (line.substr(0, 2) == "v "){
				std::istringstream s(line.substr(2));
				glm::vec3 v;
				s >> v.x >> v.y >> v.z;
				vertices.push_back(v);
			}
			else if (line.substr(0, 3) == "vt "){
				std::istringstream s(line.substr(3));
				glm::vec2 v;
				s >> v.x >> v.y;
				uvs.push_back(v);
			}
			else if (line.substr(0, 3) == "vn "){
				std::istringstream s(line.substr(3));
				glm::vec3 v;
				s >> v.x >> v.y >> v.z;
				normals.push_back(v);
			}
			else if (line.substr(0, 2) == "f ") {
				std::istringstream s(line.substr(2));
				std::string item, face;
				int i = 0;
				while (getline(s, face, ' ')){
					std::istringstream s1(face.substr(0));
					while (getline(s1, item, '/')){
						if (item.size() && i == 0 && stoi(item) > 0 && stoi(item) <= vertices.size())
							indices.push_back(stoi(item) - 1);
						else if (item.size() && i == 1 && stoi(item) > 0 && stoi(item) <= uvs.size())
							uvIndices.push_back(stoi(item) - 1);
						else if (item.size() && i == 2 && stoi(item) > 0 && stoi(item) <= normals.size())
							normalIndices.push_back(stoi(item) - 1);
						else{
							std::cerr << "Invalid index on " << filename << std::endl;
							exit(1);
						}
						i = ++i % 3;
					}
				}
			}
		}

		for (unsigned int i = 0; i < indices.size(); i++){
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

	void loadPLY(char *objFile, Vertex ***vertices, unsigned int *vertexcount, bool *normals, bool *uvs) {
		PlyFile * input = read_ply(fopen(objFile, "rb"));
		Face ** faces;
		unsigned int facecount;
		int i, j;

		// go through the element types
		for (i = 0; i < input->num_elem_types; i++) {
			int count;

			// setup the element for reading and get the element count
			char* element = setup_element_read_ply(input, i, &count);

			// get vertices
			if (strcmp("vertex", element) == 0) {
				*vertices = (Vertex**)malloc(sizeof(Vertex)* count);
				*vertexcount = count;

				// run through the properties and store them
				for (j = 0; j < input->elems[i]->nprops; j = j + 1) {
					PlyProperty* property = input->elems[i]->props[j];
					PlyProperty setup;
					setup.name = property->name;
					setup.internal_type = Float32;
					setup.count_internal = 0;
					setup.count_offset = 0;

					if (strcmp("x", property->name) == 0 && property->is_list == PLY_SCALAR) {
						setup.offset = offsetof(Vertex, XYZ[0]);
						setup_property_ply(input, &setup);
					}
					else if (strcmp("y", property->name) == 0 && property->is_list == PLY_SCALAR) {
						setup.offset = offsetof(Vertex, XYZ[1]);
						setup_property_ply(input, &setup);
					}
					else if (strcmp("z", property->name) == 0 && property->is_list == PLY_SCALAR) {
						setup.offset = offsetof(Vertex, XYZ[2]);
						setup_property_ply(input, &setup);
					}
					else if (strcmp("nx", property->name) == 0 && property->is_list == PLY_SCALAR) {
						setup.offset = offsetof(Vertex, N[0]);
						setup_property_ply(input, &setup);
						*normals = 1;
					}
					else if (strcmp("ny", property->name) == 0 && property->is_list == PLY_SCALAR) {
						setup.offset = offsetof(Vertex, N[1]);
						setup_property_ply(input, &setup);
						*normals = 1;
					}
					else if (strcmp("nz", property->name) == 0 && property->is_list == PLY_SCALAR) {
						setup.offset = offsetof(Vertex, N[2]);
						setup_property_ply(input, &setup);
						*normals = 1;
					}
					else if (strcmp("s", property->name) == 0 && property->is_list == PLY_SCALAR) {
						setup.offset = offsetof(Vertex, UV[0]);
						setup_property_ply(input, &setup);
						*uvs = 1;
					}
					else if (strcmp("t", property->name) == 0 && property->is_list == PLY_SCALAR) {
						setup.offset = offsetof(Vertex, UV[1]);
						setup_property_ply(input, &setup);
						*uvs = 1;
					}
				}

				// copy the data
				for (j = 0; j < count; j = j + 1) {
					(*vertices)[j] = (Vertex*)malloc(sizeof(Vertex));
					get_element_ply(input, (void*)((*vertices)[j]));
				}
			}
/*			// get faces
			else if (strcmp("face", element) == 0) {
				faces = (Face**)malloc(sizeof(Face)* count);
				facecount = count;

				// run through the properties and store them
				for (j = 0; j < input->elems[i]->nprops; j = j + 1) {
					PlyProperty* property = input->elems[i]->props[j];
					PlyProperty setup;
					setup.name = property->name;

					if (strcmp("vertex_indices", property->name) == 0 &&
						property->is_list == PLY_LIST) {

						setup.internal_type = Uint32;
						setup.offset = offsetof(Face, vertices);
						setup.count_internal = Uint32;
						setup.count_offset = offsetof(Face, count);

						setup_property_ply(input, &setup);
					}
					else if (strcmp("nx", property->name) == 0 &&
						property->is_list == PLY_SCALAR) {

						setup.internal_type = Float32;
						setup.offset = offsetof(Face, nx);
						setup.count_internal = 0;
						setup.count_offset = 0;

						setup_property_ply(input, &setup);
					}
					else if (strcmp("ny", property->name) == 0 &&
						property->is_list == PLY_SCALAR) {

						setup.internal_type = Float32;
						setup.offset = offsetof(Face, ny);
						setup.count_internal = 0;
						setup.count_offset = 0;

						setup_property_ply(input, &setup);
					}
					else if (strcmp("nz", property->name) == 0 &&
						property->is_list == PLY_SCALAR) {

						setup.internal_type = Float32;
						setup.offset = offsetof(Face, nz);
						setup.count_internal = 0;
						setup.count_offset = 0;

						setup_property_ply(input, &setup);
					}
					// dunno what it is
					else {
						fprintf(stderr, "unknown property type found in %s: %s\n",
							element, property->name);
					}
				}

				// do this if you want to grab the other data
				// list_pointer = get_other_properties_ply
				//                (input, offsetof(Face, struct_pointer));

				// copy the data
				for (j = 0; j < count; j = j + 1) {
					(*faces)[j] = (Face*)malloc(sizeof(Face));

					get_element_ply(input, (void*)((*faces)[j]));
				}
			}*/
		}
	}


	std::vector<glm::vec4> calculateTangents(std::vector<glm::vec3> &_vertices, std::vector<glm::vec2> &_uvs, std::vector<glm::vec3> &_normals){
		std::vector<glm::vec4> tangents;

		for (int i = 0; i < _vertices.size(); i +=3){
			glm::vec4 T;

			// Shortcuts for vertices
			glm::vec3 & v1 = _vertices[i + 0];
			glm::vec3 & v2 = _vertices[i + 1];
			glm::vec3 & v3 = _vertices[i + 2];

			// Shortcuts for UVs
			glm::vec2 & uv1 = _uvs[i + 0];
			glm::vec2 & uv2 = _uvs[i + 1];
			glm::vec2 & uv3 = _uvs[i + 2];

			T = glm::vec4(((uv3.y - uv1.y) * (v2 - v1) - (uv2.y - uv1.y) * (v3 - v1)) / ((uv2.x - uv1.x) * (uv3.y - uv1.y) - (uv2.y - uv1.y) * (uv3.x - uv1.x)), 0);

		}
	}


	void loadMaterial(char* filename, glm::vec3 &ambient, glm::vec3 &diffuse, glm::vec3 &specular, float &shininess){
		std::ifstream in(filename, std::ios::in);
		if (!in){
			std::cerr << "Cannot open " << filename << std::endl;
			exit(1);
		}

		std::string line;
		while (getline(in, line)) {
			if (line.substr(0, 3) == "Ka "){
				std::istringstream s(line.substr(3));
				s >> ambient.x >> ambient.y >> ambient.z;
			}
			else if (line.substr(0, 3) == "Kd "){
				std::istringstream s(line.substr(3));
				s >> diffuse.x >> diffuse.y >> diffuse.z;
			}
			else if (line.substr(0, 3) == "Ks "){
				std::istringstream s(line.substr(3));
				s >> specular.x >> specular.y >> specular.z;
			}
			else if (line.substr(0, 3) == "Ns "){
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
