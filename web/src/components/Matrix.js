import React from 'react'
import * as d3 from 'd3'

function initMatrix(nodes) {
  const matrix = []
  for (let i = 0; i < nodes.length; i++) {
      const node = {
          ...nodes[i],
          index: i,
          srcCount: 0,
          destCount: 0
      }
      matrix[i] = d3.range(nodes.length)
          .map((j) => ({x: j, y: i, z: 0}))
  }
  return matrix
}

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
  const emptyMatrix = initMatrix(graphHistoryPoint.nodes)
  const matrix = computeMatrix(graphHistoryPoint.nodes, emptyMatrix)

  const margin = {
    top: 80,
    right: 0,
    bottom: 10,
    left: 80
  }
  const width = 36
  const height = 36
  const n = graphHistoryPoint.nodes.length

  const z = d3.scaleLinear()
    .domain([0, colorUpperLimit])
    .clamp(true)

  return (
    <svg
      width={width * n + margin.left + margin.right}
      height={height * n + margin.top + margin.bottom}>
        <g transform={"translate(" + margin.left + "," + margin.top + ")"}>
          {
            matrix.map((node, nodeIdx) => (
              <g
                key={nodeIdx}
                className="row"
                transform={"translate(0," + (width * nodeIdx) + ")"}>
                {
                  node.map((cell, cellIdx) => (
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
                <text x={-6} y={width / 2} dy=".32em" textAnchor="end">{nodeIdx}</text>
              </g>
            ))
          }
          {
            matrix.map((node, nodeIdx) => (
              <g
                key={nodeIdx}
                className="column"
                transform={"translate(" + (width * nodeIdx) + ")rotate(-90)"}>
                <line x1={-width} />
                <text x={6} y={width / 2} dy=".32em" textAnchor="start">{nodeIdx}</text>
              </g>
            ))
          }
        </g>
    </svg>
  )
}
