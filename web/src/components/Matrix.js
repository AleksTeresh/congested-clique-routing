import React from 'react'
import * as d3 from 'd3'

import { initMatrix } from '../util/matrix'

function computeMatrix(nodes, matrix) {
  // Convert links to matrix count character occurrences.
  for (let i = 0; i < nodes.length; i++) {
      const node = nodes[i]
      for (let j = 0; j < node.messages.length; j++) {
          const message = node.messages[j]
          matrix[node.id][message.finalDest].z++
          nodes[node.id].srcCount++
          nodes[message.finalDest].destCount++
      }
  }
  return matrix
}

export default function Matrix({
  graphHistoryPoint,
  colorUpperLimit
}) {
  const { nodes } = graphHistoryPoint
  const initMatrixRow = (rowIdx) => d3.range(nodes.length)
    .map((j) => ({x: j, y: rowIdx, z: 0}))
  const emptyMatrix = initMatrix(
    graphHistoryPoint.nodes,
    initMatrixRow
  )
  const matrix = computeMatrix(nodes, emptyMatrix)

  const margin = {
    top: 30,
    right: 0,
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
      width={width * n + margin.left + margin.right}
      height={height * n + margin.top + margin.bottom}>
        <g transform={"translate(" + margin.left + "," + margin.top + ")"}>
          {
            matrix.map((cells, rowIdx) =>
              <g
                key={rowIdx}
                className="row"
                transform={"translate(0," + (width * rowIdx) + ")"}>
                {
                  cells.map((cell, cellIdx) => (
                    <rect
                      key={cellIdx}
                      className="cell"
                      x={width * cellIdx}
                      width={width}
                      height={height}
                      fill="black"
                      fillOpacity={z(cell.z)} />
                  ))
                }
                <line x2={width} />
                <text x={-6} y={width / 2} dy=".32em" textAnchor="end">{rowIdx}</text>
              </g>
            )
          }
          {
            matrix.map((col, colIdx) => (
              <g
                key={colIdx}
                className="column"
                transform={"translate(" + (width * colIdx) + ")rotate(-90)"}>
                <line x1={-width} />
                <text x={6} y={width / 2} dy=".32em" textAnchor="start">{colIdx}</text>
              </g>
            ))
          }
        </g>
    </svg>
  )
}
