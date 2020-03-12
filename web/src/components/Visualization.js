import React from 'react'

import Matrix from './Matrix'
import DataColumn from './DataColumn'

export default function Visualization({
  currRoundData,
  maxMessagesSize,
  groupedData,
  setSize
}) {
  const maxCellSize = 32
  const mainMatrixSize = 600
  const secondaryMatrixSize = mainMatrixSize / 5
  return (
    <div>
      <DataColumn
        nodes={currRoundData.nodes}
        colorUpperLimit={maxMessagesSize}
        propertyToVisualize={'messages'}
        cellSize={Math.min(
          maxCellSize,
          mainMatrixSize / (setSize * setSize))} />
      <Matrix
        graphHistoryPoint={currRoundData}
        colorUpperLimit={setSize * setSize}
        cellSize={Math.min(
          maxCellSize,
          mainMatrixSize / (setSize * setSize))} />
      <Matrix
        graphHistoryPoint={groupedData}
        colorUpperLimit={setSize * setSize * setSize}
        cellSize={Math.min(
          maxCellSize,
          secondaryMatrixSize / (setSize))} />
    </div>
  )
}
