import React, { useState, useEffect } from 'react'
import * as d3 from 'd3'
import { computeRouting } from './cppClient'

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

function Matrix({
  graphHistoryPoint
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
    .domain([0, n])
    .clamp(true)

  return (
    <svg
      width={width * n + margin.left + margin.right}
      height={height * n + margin.top + margin.bottom}>
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
              <text x={-6} y={width / 2} dy=".32em" textAnchor="end">{node.id}</text>
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
              <text x={6} y={width / 2} dy=".32em" textAnchor="start">{node.id}</text>
            </g>
          ))
        }
    </svg>
  )
}

const NUM_OF_ROUNDS = 16

function App() {
  const [tentativeSetSize, setTentativeSetSize] = useState(4)
  const [setSize, setSetSize] = useState(4)
  const [round, setRound] = useState(0)
  const [data, setData] = useState([{ nodes: [] }]) 

  const loadData = (setSize) => {
    InitRuntime().then((Module) => {
      const d = computeRouting(Module, setSize)
      setData(d)
    })
  }

  useEffect(() => {
    loadData(setSize)
  }, [])

  const handlePrev = () => setRound(round - 1)
  const handleNext = () => setRound(round + 1)

  return (
    <div className="App">
      <Matrix graphHistoryPoint={data[round]} />
      <div>
        <button
          disabled={round === 0}
          onClick={handlePrev}
          id="prevBtn">Prev</button>
        <button
          disabled={round === NUM_OF_ROUNDS}
          onClick={handleNext}
          id="nextBtn">Next</button>
      </div>
      <div>
        <label id="roundLabel"></label>
      </div>
      <div>
        <label id="numberOfNodes">{'Round ' + tentativeSetSize}</label>
        <input
          onInput={(ev) => setTentativeSetSize(Number(ev.target.value))}
          type="range"
          min="1"
          max="9"
          value={tentativeSetSize}
          className="slider"
          id="nodesRange" />
      </div>
      <div>
        <button
          onClick={() => {
            setSetSize(Number(tentativeSetSize))
            loadData(Number(tentativeSetSize))
          }}
          id="computeBtn">Compute</button>
      </div>
    </div>
  );
}

export default App