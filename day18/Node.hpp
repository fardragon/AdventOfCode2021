//
// Created by fardragon on 12/18/21.
//

#ifndef ADVENTOFCODE2021_NODE_HPP
#define ADVENTOFCODE2021_NODE_HPP

#include <variant>
#include <utility>
#include <cstdint>
#include <memory>

enum class NodeOrientation
{
	Left,
	Right,
	Top
};

class Node
{
public:
	using NodeValue = std::uint8_t;
	using NodeNested = std::pair<std::unique_ptr<Node>, std::unique_ptr<Node>>;

	Node(const NodeOrientation &orientation, NodeValue value);
	Node(const NodeOrientation &orientation, NodeNested nestedNodes);

	Node(const Node &) = delete;
	Node(Node &&) = delete;
	Node &operator=(const Node &) = delete;
	Node &operator=(Node &&) = delete;


	void Reduce();
	std::uint64_t GetMagnitude();


	void SetParent(Node *newParent);
	void SetOrientation(NodeOrientation orientation);
	void SetNested(NodeNested nested);
	void AddValue(NodeValue add);
	[[nodiscard]] NodeValue GetValue() const;

	bool IsLeftChild(Node *node);
	bool IsRightChild(Node *node);
	[[nodiscard]] bool IsValueNode() const;

	Node *GetLeftChild();
	Node *GetRightChild();

	std::unique_ptr<Node> Copy();

private:
	bool Explode();
	bool Split();
	bool ExplodeInternal(std::uint8_t depth);
	Node *FindLeft();
	Node *FindRight();

private:
	NodeOrientation nodeOrientation;
	std::variant<NodeValue, NodeNested> value;
	Node *parent = nullptr;
};

class NodeParser
{
public:

	std::unique_ptr<Node> Parse(const std::string &nodeStr);

private:
	std::unique_ptr<Node> ParseNode(const NodeOrientation &orientation, Node *parent);

private:
	std::string::const_iterator position;
};

#endif //ADVENTOFCODE2021_NODE_HPP
