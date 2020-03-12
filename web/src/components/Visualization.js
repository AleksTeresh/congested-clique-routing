import React from 'react'

import Matrix from './Matrix'
import DataColumn from './DataColumn'

export default function Visualization({
  currRoundData,
  maxMessagesSize,
  groupedData,
  setSize
}) {
  return (
    <>
      <DataColumn
        nodes={currRoundData.nodes}
        colorUpperLimit={maxMessagesSize}
        propertyToVisualize={'messages'} />
      <Matrix
        graphHistoryPoint={currRoundData}
        colorUpperLimit={setSize * setSize} />
      <Matrix
        graphHistoryPoint={groupedData}
        colorUpperLimit={setSize * setSize * setSize}/>
    </>
  )
}
