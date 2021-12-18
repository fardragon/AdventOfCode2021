//
// Created by fardragon on 12/18/21.
//

#include "Node.hpp"


Node::Node(const NodeOrientation &orientation, Node::NodeValue value)
		: nodeOrientation(orientation), value(value)
{
}

Node::Node(const NodeOrientation &orientation, Node::NodeNested nestedNodes)
		: nodeOrientation(orientation), value(std::move(nestedNodes))
{
}

bool Node::Explode()
{
	if (nodeOrientation != NodeOrientation::Top)
	{
		throw std::runtime_error("Explosion can only start from the top node");
	}

	return ExplodeInternal(0);
}

void Node::SetParent(Node *newParent)
{
	this->parent = newParent;
}

void Node::SetNested(Node::NodeNested nested)
{
	this->value = std::move(nested);
}

bool Node::ExplodeInternal(std::uint8_t depth)
{
	if (this->IsValueNode())
	{
		// value node cannot explode
		return false;
	}

	auto &nodes = std::get<NodeNested>(value);
	if (depth >= 4)
	{
		if (auto left = FindLeft(); left != nullptr)
		{
			left->AddValue(nodes.first->GetValue());
		}

		if (auto right = FindRight(); right != nullptr)
		{
			right->AddValue(nodes.second->GetValue());
		}

		value.emplace<NodeValue>(0);
		return true;
	}
	else if ((nodes.first->ExplodeInternal(depth + 1))
	         || (nodes.second->ExplodeInternal(depth + 1)))
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool Node::Split()
{
	if (this->IsValueNode())
	{
		if(this->GetValue() > 9)
		{
			auto lValue = this->GetValue() / 2;
			auto rValue = this->GetValue() - lValue;

			auto leftNode = std::make_unique<Node>(NodeOrientation::Left, lValue);
			auto rightNode = std::make_unique<Node>(NodeOrientation::Right, rValue);

			leftNode->SetParent(this);
			rightNode->SetParent(this);

			this->value.emplace<NodeNested>(std::make_pair(std::move(leftNode), std::move(rightNode)));
			return true;
		}
		else
		{
			return false;
		}
	}
	else
	{
		return this->GetLeftChild()->Split() || this->GetRightChild()->Split();
	}
}


Node *Node::FindLeft()
{
	auto current = this;
	auto lParent = this->parent;

	while(lParent != nullptr && lParent->IsLeftChild(current))
	{
		current = lParent;
		lParent = lParent->parent;
	}

	if (lParent == nullptr)
	{
		return nullptr;
	}

	auto left = lParent->GetLeftChild();
	while (!left->IsValueNode())
	{
		left = left->GetRightChild();
	}

	return left;
}

Node *Node::FindRight()
{
	auto current = this;
	auto lParent = this->parent;

	while(lParent != nullptr && lParent->IsRightChild(current))
	{
		current = lParent;
		lParent = lParent->parent;
	}

	if (lParent == nullptr)
	{
		return nullptr;
	}

	auto right = lParent->GetRightChild();
	while (!right->IsValueNode())
	{
		right = right->GetLeftChild();
	}

	return right;
}

void Node::AddValue(Node::NodeValue add)
{
	if (std::holds_alternative<NodeValue>(value))
	{
		std::get<NodeValue>(value) += add;
	}
	else
	{
		throw std::runtime_error("Cannot add to nested node");
	}
}

Node::NodeValue Node::GetValue() const
{
	if (std::holds_alternative<NodeValue>(value))
	{
		return std::get<NodeValue>(value);
	}
	else
	{
		throw std::runtime_error("Cannot get value from nested node");
	}
}

bool Node::IsLeftChild(Node *node)
{
	return GetLeftChild() == node;
}

bool Node::IsRightChild(Node *node)
{
	return GetRightChild() == node;
}

Node *Node::GetLeftChild()
{
	if (std::holds_alternative<NodeNested>(value))
	{
		return std::get<NodeNested>(value).first.get();
	}
	else
	{
		throw std::runtime_error("Cannot get children from nested node");
	}
}

Node *Node::GetRightChild()
{
	if (std::holds_alternative<NodeNested>(value))
	{
		return std::get<NodeNested>(value).second.get();
	}
	else
	{
		throw std::runtime_error("Cannot get children from nested node");
	}
}

bool Node::IsValueNode() const
{
	return std::holds_alternative<NodeValue>(value);
}

void Node::Reduce()
{
	while(Explode() || Split())
	{
	}
}

void Node::SetOrientation(NodeOrientation orientation)
{
	this->nodeOrientation = orientation;
}

std::uint64_t Node::GetMagnitude()
{
	if (this->IsValueNode())
	{
		return this->GetValue();
	}
	else
	{
		return 3 * this->GetLeftChild()->GetMagnitude() + 2 * this->GetRightChild()->GetMagnitude();
	}
}

std::unique_ptr<Node> Node::Copy()
{
	if (this->IsValueNode())
	{
		return std::make_unique<Node>(this->nodeOrientation, this->GetValue());
	}
	else
	{
		auto copy = std::make_unique<Node>(this->nodeOrientation, std::make_pair(nullptr, nullptr));
		auto leftCopy = this->GetLeftChild()->Copy();
		auto rightCopy = this->GetRightChild()->Copy();

		leftCopy->SetParent(copy.get());
		rightCopy->SetParent(copy.get());
		copy->SetNested(std::make_pair(std::move(leftCopy), std::move(rightCopy)));
		return copy;
	}
}


std::unique_ptr<Node> NodeParser::Parse(const std::string &nodeStr)
{
	position = nodeStr.begin();

	return ParseNode(NodeOrientation::Top, nullptr);
}

std::unique_ptr<Node> NodeParser::ParseNode(const NodeOrientation &orientation, Node *parent)
{
	if (std::isdigit(*position))
	{
		auto result = std::make_unique<Node>(orientation, static_cast<Node::NodeValue>(*position - '0'));
		result->SetParent(parent);
		std::advance(position, 1);
		return result;
	}

	auto result = std::make_unique<Node>(orientation, std::make_pair(nullptr, nullptr));

	if (*position != '[')
	{
		throw std::runtime_error("Failed to parse node");
	}
	std::advance(position, 1);
	auto leftNode = ParseNode(NodeOrientation::Left, result.get());

	if (*position != ',')
	{
		throw std::runtime_error("Failed to parse node");
	}
	std::advance(position, 1);
	auto rightNode = ParseNode(NodeOrientation::Right, result.get());

	if (*position != ']')
	{
		throw std::runtime_error("Failed to parse node");
	}
	std::advance(position, 1);

	result->SetParent(parent);
	result->SetNested(std::make_pair(std::move(leftNode), std::move(rightNode)));

	return result;
}


