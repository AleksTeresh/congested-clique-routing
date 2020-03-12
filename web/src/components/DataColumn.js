import React from 'react'
import * as d3 from 'd3'

import { initMatrix } from '../util/matrix'

function computeColumn(nodes, matrix, propertyToVisualize) {
  // Convert links to matrix count character occurrences.s
  for (let i = 0; i < nodes.length; i++) {
      const node = nodes[i]
      for (let j = 0; j < node[propertyToVisualize].length; j++) {
          const message = node[propertyToVisualize][j]
          matrix[node.id].z++
      }
  }
  return matrix
}

export default function DataColumn({
  nodes,
  colorUpperLimit,
  propertyToVisualize,
  cellSize
}) {
  const emptyMatrix = initMatrix(
    nodes,
    (rowIdx) => ({ x: 0, y: rowIdx, z: 0 })
  )
  const data = computeColumn(
    nodes,
    emptyMatrix,
    propertyToVisualize
  )

  const margin = {
    top: 30,
    right: 30,
    bottom: 10,
    left: 30
  }
  const width = cellSize
  const height = cellSize
  const n = nodes.length

  const z = d3.scaleLinear()
    .domain([0, colorUpperLimit])
    .clamp(true)

  return (
    <svg
      width={width + margin.left + margin.right}
      height={height * n + margin.top + margin.bottom}>
        <g transform={"translate(" + margin.right + "," + margin.top + ")"}>
          {
            data.map((node, nodeIdx) => (
              <g
                key={nodeIdx}
                className="row"
                transform={"translate(0," + (width * nodeIdx) + ")"}>
                <rect
                  className="cell"
                  x={4}
                  width={width}
                  height={height}
                  fill="black"
                  fillOpacity={z(node.z)} />
                <line x2={width} />
                <text x={0} y={width / 2} dy=".32em" textAnchor="end">{node.z}</text>
              </g>
            ))
          }
        </g>
    </svg>
  )
}
