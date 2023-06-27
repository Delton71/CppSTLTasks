#pragma once
#include "igraph.hpp"
#include <set>


class SetGraph: public IGraph {
public:
    SetGraph(size_t v_count);
    SetGraph(const IGraph* g);

    SetGraph(const IGraph& g) = delete;

    void AddEdge(size_t from, size_t to);
    size_t VerticesCount() const noexcept;
    void GetNextVertices(size_t vertex, std::vector<size_t>& vertices) const noexcept;
    void GetPrevVertices(size_t vertex, std::vector<size_t>& vertices) const noexcept;
    
private:
    const size_t v_count_;
    std::vector <std::set <size_t>> edges_;
};
