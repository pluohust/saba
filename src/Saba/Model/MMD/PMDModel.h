﻿#ifndef SABA_MODEL_MMD_PMDMODEL_H_
#define SABA_MODEL_MMD_PMDMODEL_H_

#include "MMDMaterial.h"
#include "MMDModel.h"

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <vector>
#include <string>
#include <algorithm>

namespace saba
{
	class PMDModel : public MMDModel
	{
	public:
		MMDNodeManager* GetNodeManager() override { return &m_nodeMan; }
		MMDIKManager* GetIKManager() override { return &m_ikSolverMan; }
		MMDBlendShapeManager* GetBlendShapeManager() { return &m_blendShapeMan; }

		size_t GetVertexCount() const override { return m_positions.size(); }
		const glm::vec3* GetPositions() const override { return &m_positions[0]; }
		const glm::vec3* GetNormals() const override { return &m_normals[0]; }
		const glm::vec2* GetUVs() const override { return &m_uvs[0]; }
		const glm::vec3* GetUpdatePositions() const override { return &m_updatePositions[0]; }
		const glm::vec3* GetUpdateNormals() const override { return &m_updateNormals[0]; }
		const glm::vec2* GetUpdateUVs() const override { return &m_uvs[0]; }

		size_t GetIndexElementSize() const override { return sizeof(uint16_t); }
		size_t GetIndexCount() const override { return m_indices.size(); }
		const void* GetIndices() const override { return &m_indices[0]; }

		size_t GetMaterialCount() const override { return m_materials.size(); }
		const MMDMaterial* GetMaterials() const override { return &m_materials[0]; }

		size_t GetSubMeshCount() const override { return m_subMeshes.size(); }
		const MMDSubMesh* GetSubMeshes() const override { return &m_subMeshes[0]; }

		virtual void Update(float elapsed);

		bool Load(const std::string& filepath, const std::string& mmdDataDir);
		void Destroy();

		const glm::vec3& GetBBoxMin() const { return m_bboxMin; }
		const glm::vec3& GetBBoxMax() const { return m_bboxMax; }

	private:

	private:
		std::vector<glm::vec3>	m_positions;
		std::vector<glm::vec3>	m_normals;
		std::vector<glm::vec2>	m_uvs;
		std::vector<glm::ivec2>	m_bones;
		std::vector<glm::vec2>	m_boneWeights;
		std::vector<glm::vec3>	m_updatePositions;
		std::vector<glm::vec3>	m_updateNormals;

		std::vector<uint16_t> m_indices;

		MMDBlendShape				m_baseShape;

		glm::vec3		m_bboxMin;
		glm::vec3		m_bboxMax;

		std::vector<MMDMaterial>	m_materials;
		std::vector<MMDSubMesh>		m_subMeshes;

		MMDNodeManagerT<MMDNode>	m_nodeMan;
		MMDIKManagerT<MMDIkSolver>	m_ikSolverMan;
		MMDBlendShapeManagerT<MMDBlendShape>	m_blendShapeMan;
	};
}

#endif // !SABA_MODEL_MMD_PMDMODEL_H_