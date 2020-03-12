import * as d3 from 'd3'

export function initMatrix(nodes, columnCount) {
  const matrix = []
  for (let i = 0; i < nodes.length; i++) {
      const node = {
          ...nodes[i],
          index: i,
          srcCount: 0,
          destCount: 0
      }

      matrix[i] = [...Array(columnCount)]
        .map((j) => ({x: j, y: i, z: 0}))
  }
  return matrix
}
