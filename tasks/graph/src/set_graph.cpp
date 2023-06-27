#include "set_graph.hpp"


SetGraph::SetGraph(size_t v_count): v_count_(v_count),
            edges_(std::vector<std::set<size_t>>(v_count)) {}

SetGraph::SetGraph(const IGraph* g): SetGraph(g->VerticesCount()) {
	std::vector<size_t> next;

	for (size_t from = 0; from < v_count_; ++from) {
		g->GetNextVertices(from, next);
		for (auto to: next) {
			edges_[from].insert(to);
        }
	}
}

void SetGraph::AddEdge(size_t from, size_t to) {
	if (from < v_count_ && to < v_count_) {
		edges_[from].insert(to);
    }
}

size_t SetGraph::VerticesCount() const noexcept {
	return v_count_;
}

void SetGraph::GetNextVertices(size_t vertex, std::vector<size_t>& vertices) const noexcept {
	vertices.clear();

	for (auto next: edges_[vertex]) {
		vertices.push_back(next);
    }
}

void SetGraph::GetPrevVertices(size_t vertex, std::vector<size_t>& vertices) const noexcept {
	vertices.clear();

	for (size_t prev = 0; v_count_; ++prev) {
		for (auto curr: edges_[prev]) {
			if (curr == vertex) {
				vertices.push_back(prev);
            }
		}
	}
}
