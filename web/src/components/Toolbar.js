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
  const buttonStyle = { margin: '20px', marginTop: '5px', fontSize: '15px' }
  return (
    <div style={{ margin: '80px', marginTop: '20px' }}>
      <div style={{ width: '500px' }}>
        <button
          style={buttonStyle}
          disabled={round === 0}
          onClick={handlePrev}
          id="prevBtn">
          Prev round
        </button>
        <button
          style={buttonStyle}
          disabled={round === NUM_OF_ROUNDS}
          onClick={handleNext}
          id="nextBtn">
          Next round
        </button>
        <div style={{ float: 'right', marginTop: '5px'}}>
          <label>{'Round No: ' + round}</label>
        </div>
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
      <div>
        <button
          style={buttonStyle}
          onClick={() => {
            setSetSize(Number(tentativeSetSize))
            loadData(Number(tentativeSetSize))
          }}
          id="computeBtn">Compute routing</button>
      </div>
    </div>
  )
}
