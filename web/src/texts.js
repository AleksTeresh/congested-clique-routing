
const step2Title = `**Step 2. Move the messages such that each such subset W holds
exactly |W||W'| = n messages for each subset W'**`
const step3Title = `**Step 3. For each pair of subsets W, W'
, move all messages
destined to nodes in W' within W such that each node
in W holds exactly |W'
| =
√
n messages with
destinations in W'**`
const step5Title = `**Step 5. For each W, move all messages within W to their
destinations**`
const texts = {
  algorithmSteps: [
    {
      title: '**Step 1. Partition nodes.**',
      detail: 'Partition the nodes into the disjoint subsets of the size sqrt(n), where n is the total number of nodes on the clique.'
    },
    {
      title: step2Title,
      detail: `Substep 1. Each subset W computes, for each set W'
      , the number
      of messages its constituents hold in total for nodes in
      W'
      . The results are announced to all nodes.\n\rCan be done in 2 rounds`
    },
    {
      title: step2Title,
      detail: `Substep 1. Each subset W computes, for each set W'
      , the number
      of messages its constituents hold in total for nodes in
      W'
      . The results are announced to all nodes.\n\rSecond round`
    },
    {
      title: step2Title,
      detail: `Substep 2. All nodes locally compute a pattern according to which
      the messages are to be moved between the sets. It
      satisfies that from each set W to each set W'
      , n
      messages need to be sent, and that in the resulting
      configuration, each subset W holds exactly
      |W||W'
      | = n messages for each subset W'\n\rSubstep 3. All nodes in subset W announce to all other nodes in
      W the number of messages they need to move to each
      set W'
      according to the previous step.`
    },
    {
      title: step2Title,
      detail: `Substep 3. All nodes in subset W announce to all other nodes in
      W the number of messages they need to move to each
      set W'
      according to the previous step.\n\rSecond round`
    },
    {
      title: step2Title,
      detail: `Substep 4. All nodes in W compute a pattern for moving messages
      within W so that the resulting distribution permits to
      realize the exchange computed in Substep 2 in a single
      round (i.e., each node in W must hold exactly
      |W'
      | =
      √
      n messages with (intermediate) destinations in
      W'
      ).\n\r Substep 5. The redistribution within the sets according to Substep 4 is
      executed.`
    },
    {
      title: step2Title,
      detail: `Substep 5.The redistribution within the sets according to Substep 4 is
      executed.\n\rSecond round`
    },
    {
      title: step2Title,
      detail: `Substep 6. The redistribution among the sets computed in Substep 2
      is executed.`
    },
    {
      title: step3Title,
      detail: `First,
      each node in W announces to each other node in W the
      number of messages it holds for each set W'.This can be done in 2 rounds`
    },
    {
      title: step3Title,
      detail: `First,
      each node in W announces to each other node in W the
      number of messages it holds for each set W'.This can be done in 2 rounds.\n\r Second round`
    },
    {
      title: step3Title,
      detail: `With the information obtained in the last 2 rounds, the nodes in W can deterministically compute (intermediate) destinations for each message
      in W such that the resulting distribution of messages meets
      the condition imposed by Step 3. This can be done in 2 rounds.`
    },
    {
      title: step3Title,
      detail: `With the information obtained in the last 2 rounds, the nodes in W can deterministically compute (intermediate) destinations for each message
      in W such that the resulting distribution of messages meets
      the condition imposed by Step 3. This can be done in 2 rounds.\n\rSecond round`
    },
    {
      title: `**Step 4. For each pair of subsets W, W'
      , move all messages
      destined to nodes in W'
      from W to W'**`,
      detail: `Can be executed in a single round by
      each node in W sending exactly one of the messages with
      destination in W'
      it holds to each node in W'`
    },
    {
      title: step5Title,
      detail: `Each node in W sends to each other node in W information about how many messages it has destined to each node in W. This can be done in 2 rounds.`
    },
    {
      title: step5Title,
      detail: `Each node in W sends to each other node in W information about how many messages it has destined to each node in W. This can be done in 2 rounds.\n\rSecond round`
    },
    {
      title: step5Title,
      detail: `With the information obtained in the last 2 rounds, each node can deliver all the messages to their final destinations. This can be done in 2 rounds.`
    },
    {
      title: step5Title,
      detail: 'With the information obtained in the last 2 rounds, each node can deliver all the messages to their final destinations. This can be done in 2 rounds.\n\rSecond round.'
    }
  ],
  editInstructions: '**Click** a cell to add a message between corresponding nodes.\n\r**Hold Shift and click** a cell, to remove messages.'
}
export default texts
