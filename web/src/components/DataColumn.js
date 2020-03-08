import React from 'react'
import * as d3 from 'd3'

function initMatrix(nodes) {
  const matrix = []
  for (let i = 0; i < nodes.length; i++) {
      const node = {
          ...nodes[i],
          index: i,
          count: 0,
      }
      matrix[i] = {x: 0, y: i, z: 0}
  }
  return matrix
}

function computeMatrix(nodes, matrix, propertyToVisualize) {
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
  propertyToVisualize
}) {
  const emptyMatrix = initMatrix(nodes)
  const data = computeMatrix(
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
  const width = 36
  const height = 36
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
