import React from 'react'
import * as d3 from 'd3'

export default function Matrix({
  data,
  colorUpperLimit,
  cellSize
}) {
  const margin = {
    top: 30,
    right: 0,
    bottom: 10,
    left: 30
  }

  const n = data.length
  const columnCount = data[0]
    ? data[0].length
    : 0
  const width = cellSize
  const height = cellSize

  const z = d3.scaleLinear()
    .domain([0, colorUpperLimit])
    .clamp(true)
  return (
    <svg
      style={{ alignSelf: 'flex-end' }}
      width={width * columnCount + margin.left + margin.right}
      height={height * n + margin.top + margin.bottom}>
        <g transform={"translate(" + margin.left + "," + margin.top + ")"}>
          {
            data.map((cells, rowIdx) =>
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
                <text x={-6} y={width / 2} dy={`.${cellSize}em`} 
                  fontSize={cellSize / 2} textAnchor="end">{rowIdx}</text>
              </g>
            )
          }
          {
            data.length === 0
            ? null
            : data[0].map((col, colIdx, arr) => (
                <g
                  key={colIdx}
                  className="column"
                  transform={"translate(" + (width * colIdx) + ")rotate(-90)"}>
                  <line x1={-width} />
                  {
                    arr.length === 1
                    ? null
                    : <text
                      x={6} 
                      y={width / 2}
                      dy={`.${cellSize}em`} 
                      fontSize={cellSize / 2}
                      textAnchor="start">
                        {colIdx}
                    </text>
                  }
                </g>
              ))
          }
        </g>
    </svg>
  )
}
