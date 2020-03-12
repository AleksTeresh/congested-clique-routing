import React, { useState } from 'react'

const MAX_NUM_OF_ROUNDS = 16

const buttonStyle = {
  margin: '20px',
  marginTop: '5px',
  fontSize: '15px',
  lineHeight: '2.5',
  padding: '0 20px',
  fontSize: '1rem',
  border: 0,
  borderRadius: '10px',
  backgroundColor: '#f5f4f2',
  boxShadow: 'inset 1px 1px 1px rgba(255, 255, 255, .6), inset -1px -1px 1px rgba(0, 0, 0, .6)'
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
      <input
        type="button"
        style={buttonStyle}
        disabled={round === 0}
        onClick={handlePrev}
        value="Prev round"
        id="prevBtn" />
      <input
        type="button"
        style={buttonStyle}
        disabled={round === MAX_NUM_OF_ROUNDS}
        onClick={handleNext}
        value="Next round"
        id="nextBtn"/>
      <div style={roundIndicatorStyle}>
        <label>{'Round No: ' + round}</label>
      </div>
    </div>
  )
}
