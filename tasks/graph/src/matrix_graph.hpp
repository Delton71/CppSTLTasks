#pragma once
#include "igraph.hpp"


class MatrixGraph: public IGraph {
public:
    MatrixGraph(size_t v_count);
    MatrixGraph(const IGraph* g);

    MatrixGraph(const IGraph& g) = delete;

    void AddEdge(size_t from, size_t to);
    size_t VerticesCount() const noexcept;
    void GetNextVertices(size_t vertex, std::vector<size_t>& vertices) const noexcept;
    void GetPrevVertices(size_t vertex, std::vector<size_t>& vertices) const noexcept;
    
private:
    size_t v_count_;
    std::vector <std::vector <bool>> matrix_;
};
