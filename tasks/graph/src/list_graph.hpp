#pragma once
#include "igraph.hpp"


class ListGraph: public IGraph {
public:
    ListGraph(size_t v_count);
    ListGraph(const IGraph* g);

    ListGraph(const IGraph& g) = delete;

    void AddEdge(size_t from, size_t to);
    size_t VerticesCount() const noexcept;
    void GetNextVertices(size_t vertex, std::vector<size_t>& vertices) const noexcept;
    void GetPrevVertices(size_t vertex, std::vector<size_t>& vertices) const noexcept;
    
private:
    const size_t v_count_;
    std::vector <std::vector <size_t>> edges_;
};
