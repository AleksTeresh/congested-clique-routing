import React from 'react'
import * as d3 from 'd3'

export default function Matrix({
  data,
  colorUpperLimit,
  cellSize,
  editMode,
  addToCustomInput,
  removeFromCustomInput,
  hide
}) {
  const svgStyle = {
    alignSelf: 'flex-end',
    visibility: hide ? 'hidden': ''
  }
  const columnCount = data[0]
    ? data[0].length
    : 0
  const columnVector = columnCount === 1
  const margin = {
    top: cellSize,
    right: columnVector ? 0 : (cellSize),
    bottom: cellSize,
    left: columnVector ? 0 : (cellSize)
  }
  const textShift = cellSize / (32 / 6)
  const n = hide
    ? 1 // if matrix is hidden, its height should not push other elements down
    : data.length
  const width = cellSize
  const height = cellSize

  const z = d3.scaleLinear()
    .domain([0, colorUpperLimit])
    .clamp(true)

  const sent = data.map(p => p.reduce((a, b) => a + b, 0))
  const received = data.map((_, i) => data.reduce((a, b) => a + b[i], 0))
  return (
    <svg
      style={svgStyle}
      width={width * columnCount + margin.left + margin.right}
      height={height * n + margin.top + margin.bottom}>
        <g transform={"translate(" + (columnVector ? 0 : margin.left) + "," + margin.top + ")"}>
          {
            data.map((cells, rowIdx) =>
              <g
                key={rowIdx}
                className="row"
                transform={"translate(0," + (width * rowIdx) + ")"}>
                {
                  cells.map((cell, cellIdx) => (
                    <rect
                      onClick={(e) => {
                        if (editMode) {
                          e.shiftKey
                          ? removeFromCustomInput(rowIdx, cellIdx)
                          : addToCustomInput(rowIdx, cellIdx) 
                        }
                      }}
                      key={cellIdx}
                      className="cell"
                      x={width * cellIdx}
                      width={width}
                      height={height}
                      fill="black"
                      fillOpacity={z(cell)} />
                  ))
                }
                <line x2={width * columnCount} stroke={'#999999'} strokeWidth={0.5} />
                {
                  columnVector
                  ? null
                  : <text x={-textShift} y={width / 2} dy={`.${cellSize.toFixed(0)}em`} 
                  fontSize={cellSize / 2} textAnchor="end">{rowIdx}</text>
                }
                {
                  editMode
                  ? <text
                      x={textShift + width * n}
                      y={width / 2}
                      dy={`.${cellSize.toFixed(0)}em`}
                      fontSize={cellSize / 2}
                      textAnchor="end">
                      {sent[rowIdx]}
                    </text>
                  : null
                }
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
                  <line x1={-width * columnCount} stroke={'#999999'} strokeWidth={0.5} />
                  {
                    arr.length === 1
                    ? null
                    : <text
                      x={textShift} 
                      y={width / 2}
                      dy={`.${cellSize.toFixed(0)}em`}
                      fontSize={cellSize / 2}
                      textAnchor="start">
                        {colIdx}
                    </text>
                  }
                  {
                    editMode
                    ? <text
                        transform="rotate(90)"
                        x={width / 2 + textShift}
                        y={height * n + textShift}
                        dy={`.${cellSize.toFixed(0)}em`}
                        fontSize={cellSize / 2}
                        textAnchor="end">
                        {received[colIdx]}
                      </text>
                    : null
                }
                </g>
              ))
          }
        </g>
    </svg>
  )
}
