import React from 'react'

const NUM_OF_ROUNDS = 16

export default function Toolbar({
  round,
  handlePrev,
  handleNext,
  tentativeSetSize,
  setTentativeSetSize,
  setSetSize,
  loadData
}) {
  return (
    <div style={{ margin: '80px', marginTop: '20px' }}>
      <div style={{ margin: '20px' }}>
        <button
          style={{ margin: '5px' }}
          disabled={round === 0}
          onClick={handlePrev}
          id="prevBtn">Prev round</button>
        <button
          style={{ margin: '5px' }}
          disabled={round === NUM_OF_ROUNDS}
          onClick={handleNext}
          id="nextBtn">Next round</button>
      </div>
      <div>
        <label id="roundLabel"></label>
      </div>
      <div style={{ margin: '20px' }}>
        <label id="numberOfNodes">{'Size of subset ' + tentativeSetSize}</label>
        <br />
        <input
          onInput={(ev) => setTentativeSetSize(Number(ev.target.value))}
          type="range"
          min="1"
          max="9"
          value={tentativeSetSize}
          className="slider"
          id="nodesRange" />
      </div>
      <div style={{ margin: '20px' }}>
        <button
          onClick={() => {
            setSetSize(Number(tentativeSetSize))
            loadData(Number(tentativeSetSize))
          }}
          id="computeBtn">Compute routing</button>
      </div>
    </div>
  )
}
