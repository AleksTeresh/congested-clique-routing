import * as d3 from 'd3'

export function initMatrix(nodes, initMatrixEntry) {
  const matrix = []
  for (let i = 0; i < nodes.length; i++) {
      const node = {
          ...nodes[i],
          index: i,
          srcCount: 0,
          destCount: 0
      }

      matrix[i] = initMatrixEntry(i)
  }
  return matrix
}
