﻿#ifndef SABA_MODEL_MMD_MMDMODEL_H_
#define SABA_MODEL_MMD_MMDMODEL_H_

#include "MMDNode.h"
#include "MMDIkSolver.h"

#include <vector>
#include <string>
#include <algorithm>
#include <cstdint>
#include <memory>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

namespace saba
{
	struct MMDMaterial;

	struct MMDBlendShapeVertex
	{
		uint32_t	m_index;
		glm::vec3	m_position;
	};

	struct MMDBlendShape
	{
		std::string							m_name;
		float								m_weight;
		std::vector<MMDBlendShapeVertex>	m_vertices;
	};

	class MMDNodeManager
	{
	public:
		static const size_t NPos = -1;

		virtual size_t GetNodeCount() = 0;
		virtual size_t FindNodeIndex(const std::string& name) = 0;
		virtual MMDNode* GetMMDNode(size_t idx) = 0;

		MMDNode* GetMMDNode(const std::string& nodeName)
		{
			auto findIdx = FindNodeIndex(nodeName);
			if (findIdx == NPos)
			{
				return nullptr;
			}
			return GetMMDNode(findIdx);
		}
	};

	class MMDIKManager
	{
	public:
		static const size_t NPos = -1;

		virtual size_t GetIKSolverCount() = 0;
		virtual size_t FindIKSolverIndex(const std::string& name) = 0;
		virtual MMDIkSolver* GetMMDIKSolver(size_t idx) = 0;

		MMDIkSolver* GetMMDIKSolver(const std::string& ikName)
		{
			auto findIdx = FindIKSolverIndex(ikName);
			if (findIdx == NPos)
			{
				return nullptr;
			}
			return GetMMDIKSolver(findIdx);
		}
	};

	class MMDBlendShapeManager
	{
	public:
		static const size_t NPos = -1;

		virtual size_t GetBlendKeyShapeCount() = 0;
		virtual size_t FindBlendKeyShapeIndex(const std::string& name) = 0;
		virtual MMDBlendShape* GetMMDBlendKeyShape(size_t idx) = 0;

		MMDBlendShape* GetMMDBlendKeyShape(const std::string& name)
		{
			auto findIdx = FindBlendKeyShapeIndex(name);
			if (findIdx == NPos)
			{
				return nullptr;
			}
			return GetMMDBlendKeyShape(findIdx);
		}
	};

	struct MMDSubMesh
	{
		int	m_beginIndex;
		int	m_vertexCount;
		int	m_materialID;
	};

	class MMDModel
	{
	public:
		virtual MMDNodeManager* GetNodeManager() = 0;
		virtual MMDIKManager* GetIKManager() = 0;
		virtual MMDBlendShapeManager* GetBlendShapeManager() = 0;

		virtual size_t GetVertexCount() const = 0;
		virtual const glm::vec3* GetPositions() const = 0;
		virtual const glm::vec3* GetNormals() const = 0;
		virtual const glm::vec2* GetUVs() const = 0;
		virtual const glm::vec3* GetUpdatePositions() const = 0;
		virtual const glm::vec3* GetUpdateNormals() const = 0;
		virtual const glm::vec2* GetUpdateUVs() const = 0;

		virtual size_t GetIndexElementSize() const = 0;
		virtual size_t GetIndexCount() const = 0;
		virtual const void* GetIndices() const = 0;

		virtual size_t GetMaterialCount() const = 0;
		virtual const MMDMaterial* GetMaterials() const = 0;

		virtual size_t GetSubMeshCount() const = 0;
		virtual const MMDSubMesh* GetSubMeshes() const = 0;

		virtual void Update(float elapsed) = 0;

	protected:
		template <typename NodeType>
		class MMDNodeManagerT : public MMDNodeManager
		{
		public:
			using NodePtr = std::unique_ptr<NodeType>;

			size_t GetNodeCount() override { return m_nodes.size(); }

			size_t FindNodeIndex(const std::string& name) override
			{
				auto findIt = std::find_if(
					m_nodes.begin(),
					m_nodes.end(),
					[&name](const NodePtr& node) { return node->m_name == name; }
				);
				if (findIt == m_nodes.end())
				{
					return NPos;
				}
				else
				{
					return findIt - m_nodes.begin();
				}
			}

			MMDNode* GetMMDNode(size_t idx) override
			{
				return m_nodes[idx].get();
			}

			NodeType* AddNode()
			{
				m_nodes.emplace_back(std::make_unique<NodeType>());
				return m_nodes[m_nodes.size() - 1].get();
			}

			NodeType* GetNode(size_t i)
			{
				return m_nodes[i].get();
			}

			std::vector<NodePtr>* GetNodes()
			{
				return &m_nodes;
			}

		private:
			std::vector<NodePtr>	m_nodes;
		};

		template <typename IKSolverType>
		class MMDIKManagerT : public MMDIKManager
		{
		public:
			using IKSolverPtr = std::unique_ptr<IKSolverType>;

			size_t GetIKSolverCount() override { return m_ikSolvers.size(); }

			size_t FindIKSolverIndex(const std::string& name) override
			{
				auto findIt = std::find_if(
					m_ikSolvers.begin(),
					m_ikSolvers.end(),
					[&name](const IKSolverPtr& ikSolver) { return ikSolver->GetName() == name; }
				);
				if (findIt == m_ikSolvers.end())
				{
					return NPos;
				}
				else
				{
					return findIt - m_ikSolvers.begin();
				}
			}

			MMDIkSolver* GetMMDIKSolver(size_t idx)
			{
				return m_ikSolvers[idx].get();
			}

			IKSolverType* AddIKSolver()
			{
				m_ikSolvers.emplace_back(std::make_unique<IKSolverType>());
				return m_ikSolvers[m_ikSolvers.size() - 1].get();
			}

			IKSolverType* GetIKSolver(size_t i)
			{
				return m_ikSolvers[i].get();
			}

			std::vector<IKSolverPtr>* GetIKSolvers()
			{
				return &m_ikSolvers;
			}

		private:
			std::vector<IKSolverPtr>	m_ikSolvers;
		};

		template <typename BlendShapeType>
		class MMDBlendShapeManagerT : public MMDBlendShapeManager
		{
		public:
			using BlendShapePtr = std::unique_ptr<BlendShapeType>;

			size_t GetBlendKeyShapeCount() override { return m_keyShapes.size(); }

			size_t FindBlendKeyShapeIndex(const std::string& name) override
			{
				auto findIt = std::find_if(
					m_keyShapes.begin(),
					m_keyShapes.end(),
					[&name](const BlendShapePtr& blendShape) { return blendShape->m_name == name; }
				);
				if (findIt == m_keyShapes.end())
				{
					return NPos;
				}
				else
				{
					return findIt - m_keyShapes.begin();
				}
			}

			MMDBlendShape* GetMMDBlendKeyShape(size_t idx) override
			{
				return m_keyShapes[idx].get();
			}

			BlendShapeType* AddBlendKeyShape()
			{
				m_keyShapes.emplace_back(std::make_unique<BlendShapeType>());
				return m_keyShapes[m_keyShapes.size() - 1].get();
			}

			BlendShapeType* GetBlendKeyShape(size_t i)
			{
				return m_keyShapes[i].get();
			}

			std::vector<BlendShapePtr>* GetBlendKeyShapes()
			{
				return &m_keyShapes;
			}

		private:
			std::vector<BlendShapePtr>	m_keyShapes;
		};
	};
}

#endif // !SABA_MODEL_MMD_MMDMODEL_H_