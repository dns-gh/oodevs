package swapi

// Immutable party entity
type Party struct {
	id   uint32
	name string
}

func NewParty(id uint32, name string) *Party {
	return &Party{
		id:   id,
		name: name,
	}
}

func (party *Party) GetId() uint32 {
	return party.id
}

func (party *Party) GetName() string {
	return party.name
}
