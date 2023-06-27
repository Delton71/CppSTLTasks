#include "list_graph.hpp"


ListGraph::ListGraph(size_t v_count): v_count_(v_count),
            edges_(std::vector<std::vector<size_t>>(v_count)) {}

ListGraph::ListGraph(const IGraph* g): ListGraph(g->VerticesCount()) {
	std::vector<size_t> next;

	for (size_t from = 0; from < v_count_; ++from) {
		g->GetNextVertices(from, next);
		for (auto to: next) {
			edges_[from].push_back(to);
        }
	}
}

void ListGraph::AddEdge(size_t from, size_t to) {
	if (from < v_count_ && to < v_count_) {
		edges_[from].push_back(to);
    }
}

size_t ListGraph::VerticesCount() const noexcept {
	return v_count_;
}

void ListGraph::GetNextVertices(size_t vertex, std::vector<size_t>& vertices) const noexcept {
	vertices.clear();

	for (auto next: edges_[vertex]) {
		vertices.push_back(next);
    }
}

void ListGraph::GetPrevVertices(size_t vertex, std::vector<size_t>& vertices) const noexcept {
	vertices.clear();

	for (size_t prev = 0; v_count_; ++prev) {
		for (auto curr: edges_[prev]) {
			if (curr == vertex) {
				vertices.push_back(prev);
            }
		}
	}
}
