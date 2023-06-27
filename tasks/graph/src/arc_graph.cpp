#include "arc_graph.hpp"

ArcGraph::ArcGraph(size_t v_count): v_count_(v_count) {}

ArcGraph::ArcGraph(const IGraph* g): ArcGraph(g->VerticesCount()) {
    std::vector<size_t> next;

	for (size_t from = 0; from < v_count_; ++from) {
		g->GetNextVertices(from, next);
		for (size_t to: next) {
			edges_.push_back({from, to});
        }
	}
}

void ArcGraph::AddEdge(size_t from, size_t to) {
    if (from < v_count_ && to < v_count_) {
        edges_.push_back({from, to});
    }
}

size_t ArcGraph::VerticesCount() const noexcept {
    return v_count_;
}

void ArcGraph::GetNextVertices(size_t vertex, std::vector<size_t>& vertices) const noexcept {
    vertices.clear();
    for (auto e : edges_) {
        if (e.first == vertex) {
            vertices.push_back(e.second);
        }
    }
}

void ArcGraph::GetPrevVertices(size_t vertex, std::vector<size_t>& vertices) const noexcept {
    vertices.clear();
    for (auto e : edges_) {
        if (e.second == vertex) {
            vertices.push_back(e.first);
        }
    }
}
