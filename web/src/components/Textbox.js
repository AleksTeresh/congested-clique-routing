import React from 'react'

import MarkdownRender from './MarkdownRender'

export default function Textbox({
  texts,
  round
}) {
  const wrapperStyle = {
    border: '1px solid black',
    height: 'fit-content',
    display: 'inline-grid',
    padding: '10px',
    margin: '10px',
    marginTop: '30px'
  }
  return (
    <div
      style={wrapperStyle}>
      <MarkdownRender
        source={
          texts
            .algorithmSteps[round]
            .title
        } />
      <MarkdownRender
        source={
          texts
            .algorithmSteps[round]
            .detail} />
    </div>
  )
}
