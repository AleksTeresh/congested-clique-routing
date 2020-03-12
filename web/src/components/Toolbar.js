import React, { useState } from 'react'

const MAX_NUM_OF_ROUNDS = 16

const buttonStyle = {
  margin: '20px',
  marginTop: '5px',
  fontSize: '15px'
}
const wrapperStyle = {
  margin: '80px',
  marginTop: '20px'
}
const nextPrevButtonStyle = {
  width: '500px'
}
const inputControlsWrapperStyle = {
  margin: '20px'
}

export default function Toolbar({
  round,
  handlePrev,
  handleNext,
  setSize,
  setSetSize,
  loadData
}) {
  const [tentativeSetSize, setTentativeSetSize] = useState(4)

  return (
    <div style={wrapperStyle}>
      <PrevNextButtons
        round={round}
        handlePrev={handlePrev}
        handleNext={handleNext} />
      <InputControls
        setSize={setSize}
        setSetSize={setSetSize}
        tentativeSetSize={tentativeSetSize}
        setTentativeSetSize={setTentativeSetSize}
        loadData={loadData} />
    </div>
  )
}

function InputControls({
  setSize,
  setSetSize,
  tentativeSetSize,
  setTentativeSetSize,
  loadData
}) {
  return <>
    <div style={inputControlsWrapperStyle}>
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
          if (tentativeSetSize === setSize) {
            loadData(Number(tentativeSetSize))
          } else {
            setSetSize(Number(tentativeSetSize))
          }
        }}
        id="computeBtn">Compute routing</button>
    </div>
  </>
}

function PrevNextButtons({
  round,
  handlePrev,
  handleNext
}) {
  const roundIndicatorStyle = {
    float: 'right',
    marginTop: '5px'
  }
  return (
    <div style={nextPrevButtonStyle}>
      <button
        style={buttonStyle}
        disabled={round === 0}
        onClick={handlePrev}
        id="prevBtn">
        Prev round
      </button>
      <button
        style={buttonStyle}
        disabled={round === MAX_NUM_OF_ROUNDS}
        onClick={handleNext}
        id="nextBtn">
        Next round
      </button>
      <div style={roundIndicatorStyle}>
        <label>{'Round No: ' + round}</label>
      </div>
    </div>
  )
}
