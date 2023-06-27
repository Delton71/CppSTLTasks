#include "matrix_graph.hpp"

MatrixGraph::MatrixGraph(size_t v_count): v_count_(v_count),
            matrix_(std::vector<std::vector<bool>>(v_count, std::vector<bool>(v_count, false))) {}

MatrixGraph::MatrixGraph(const IGraph* g): MatrixGraph(g->VerticesCount()) {
    std::vector<size_t> next;

	for (size_t from = 0; from < v_count_; ++from) {
		g->GetNextVertices(from, next);
		for (size_t to: next) {
			matrix_[from][to] = true;
        }
	}
}

void MatrixGraph::AddEdge(size_t from, size_t to) {
    if (from < v_count_ && to < v_count_) {
        matrix_[from][to] = true;
    }
}

size_t MatrixGraph::VerticesCount() const noexcept {
    return v_count_;
}

void MatrixGraph::GetNextVertices(size_t vertex, std::vector<size_t>& vertices) const noexcept {
    vertices.clear();

    for (size_t next = 0; next < v_count_; ++next) {
        if (matrix_[vertex][next]) {
            vertices.push_back(next);
        }
    }
}

void MatrixGraph::GetPrevVertices(size_t vertex, std::vector<size_t>& vertices) const noexcept {
    vertices.clear();

    for (size_t prev = 0; prev < v_count_; ++prev) {
        if (matrix_[prev][vertex]) {
            vertices.push_back(prev);
        }
    }
}
