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
    <>
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
    </>
  )
}
