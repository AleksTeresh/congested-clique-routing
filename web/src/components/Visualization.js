import React from 'react'

import Matrix from './Matrix'

import { initMatrix } from '../util/matrix'

function computeColumn(nodes, matrix) {
  for (let i = 0; i < nodes.length; i++) {
      const node = nodes[i]
      for (let j = 0; j < node.messages.length; j++) {
          const message = node.messages[j]
          matrix[node.id][0]++
      }
  }
  return matrix
}

function computeMatrix(nodes, matrix) {
  for (let i = 0; i < nodes.length; i++) {
      const node = nodes[i]
      for (let j = 0; j < node.messages.length; j++) {
          const message = node.messages[j]
          matrix[node.id][message.finalDest]++
      }
  }
  return matrix
}

function groupPerSubclass(currRoundData) {
  const setSize = Math.round(Math.sqrt(currRoundData.nodes.length))
  const initGroupedNodes = [...(new Array(setSize))].map((p, i) => ({ id: i, messages: [] }))
  return currRoundData.nodes.reduce((acc, node, idx) => {
      const subsetId = Math.floor(idx / setSize) 
      node.messages.forEach(message => {
        const srcSet = Math.floor(message.src / setSize)
        const finalDestSet = Math.floor(message.finalDest / setSize)
        acc[subsetId].messages.push({ src: srcSet, finalDest: finalDestSet })
      })
      return acc
    }, initGroupedNodes)
}

function getMaxSizeOfProperty(data, property) {
  return data.reduce((acc, p) => Math.max(
    acc,
    p.nodes.reduce((accInner, s) =>
      Math.max(
        accInner,
        s[property].length
      ), 0)
  ), 0)
}

export default function Visualization({
  data,
  customInput,
  round,
  editMode,
  addToCustomInput,
  removeFromCustomInput
}) {
  const currRoundData = data[round]
  const nodes = currRoundData.nodes
  const setSize = Math.round(Math.sqrt(editMode ? customInput.length : nodes.length))
  const groupedNodes = groupPerSubclass(currRoundData)

  const maxMessagesSize = getMaxSizeOfProperty(data, 'messages')

  const maxCellSize = 32
  const mainMatrixSize = 550
  const secondaryMatrixSize = mainMatrixSize / 5

  const emptyColumn = initMatrix(
    nodes,
    1
  )
  const column = computeColumn(
    nodes,
    emptyColumn
  )

  const emptyMatrix = initMatrix(nodes, setSize * setSize)
  const matrix = computeMatrix(
    nodes,
    emptyMatrix
  )

  const emptyGroupedMatrix = initMatrix(groupedNodes, setSize)
  const groupedMatrix = computeMatrix(
    groupedNodes,
    emptyGroupedMatrix
  )
  return (
    <div style={{ display: 'flex' }}>
      <Matrix
        hide={editMode}
        data={column}
        colorUpperLimit={maxMessagesSize}
        cellSize={Math.min(
          maxCellSize,
          mainMatrixSize / (setSize * setSize))} />
      <Matrix
        data={editMode ? customInput : matrix}
        colorUpperLimit={setSize * setSize}
        editMode={editMode}
        addToCustomInput={addToCustomInput}
        removeFromCustomInput={removeFromCustomInput}
        cellSize={Math.min(
          maxCellSize,
          mainMatrixSize / (setSize * setSize))} />
      <Matrix
        hide={editMode}
        data={groupedMatrix}
        colorUpperLimit={setSize * setSize * setSize}
        cellSize={Math.min(
          maxCellSize,
          secondaryMatrixSize / (setSize))} />
    </div>
  )
}
